#include "Layer_Fadecandy.h"
#include <algorithm>
#include "arm_math.h"

SMLayerFadecandy::SMLayerFadecandy(fcBuffers * newfcbuffers, uint16_t width, uint16_t height) {
    fcbuffers = newfcbuffers;
    this->matrixWidth = width;
    this->matrixHeight = height;    
}

inline uint32_t SMLayerFadecandy::calculateFcInterpCoefficient()
{
    /*
     * Calculate our interpolation coefficient. This is a value between
     * 0x0000 and 0x10000, representing some point in between fbPrev and fbNext.
     *
     * We timestamp each frame at the moment its final packet has been received.
     * In other words, fbNew has no valid timestamp yet, and fbPrev/fbNext both
     * have timestamps in the recent past.
     *
     * fbNext's timestamp indicates when both fbPrev and fbNext entered their current
     * position in the keyframe queue. The difference between fbPrev and fbNext indicate
     * how long the interpolation between those keyframes should take.
     */

    if(!fcbuffers)
        return 0;

    if(fcbuffers->flags & CFLAG_NO_INTERPOLATION)
        return 0x10000;

    uint32_t now = millis();
    uint32_t tsPrev = fcbuffers->fbPrev->timestamp;
    uint32_t tsNext = fcbuffers->fbNext->timestamp;
    uint32_t tsDiff = tsNext - tsPrev;
    uint32_t tsElapsed = now - tsNext;

    // Careful to avoid overflows if the frames stop coming...
    return (std::min<uint32_t>(tsElapsed, tsDiff) << 16) / tsDiff;
}

uint32_t fcCoefficient;
uint32_t icPrev;
uint32_t icNext;

void SMLayerFadecandy::frameRefreshCallback(void) {
    // We can switch to the next frame's buffer now.
    fcbuffers->finalizeFrame();

    // Performance counter, for monitoring frame rate externally
    perf_frameCounter++;

    fcCoefficient = calculateFcInterpCoefficient();
    icPrev = 257 * (0x10000 - fcCoefficient);
    icNext = 257 * fcCoefficient;
}

ALWAYS_INLINE static inline
uint32_t lutInterpolate(const uint16_t *lut, uint32_t arg)
{
    /*
     * Using our color LUT for the indicated channel, convert the
     * 16-bit intensity "arg" in our input colorspace to a corresponding
     * 16-bit intensity in the device colorspace.
     *
     * Remember that our LUT is 257 entries long. The final entry corresponds to an
     * input of 0x10000, which can't quite be reached.
     *
     * 'arg' is in the range [0, 0xFFFF]
     *
     * This operation is equivalent to the following:
     *
     *      unsigned index = arg >> 8;          // Range [0, 0xFF]
     *      unsigned alpha = arg & 0xFF;        // Range [0, 0xFF]
     *      unsigned invAlpha = 0x100 - alpha;  // Range [1, 0x100]
     *
     *      // Result in range [0, 0xFFFF]
     *      return (lut[index] * invAlpha + lut[index + 1] * alpha) >> 8;
     *
     * This is easy to understand, but it turns out to be a serious bottleneck
     * in terms of speed and memory bandwidth, as well as register pressure that
     * affects the compilation of updatePixel().
     *
     * To speed this up, we try and do the lut[index] and lut[index+1] portions
     * in parallel using the SMUAD instruction. This is a pair of 16x16 multiplies,
     * and the results are added together. We can combine this with an unaligned load
     * to grab two adjacent entries from the LUT. The remaining complications are:
     *
     *   1. We wanted unsigned, not signed
     *   2. We still need to generate the input values efficiently.
     *
     * (1) is easy to solve if we're okay with 15-bit precision for the LUT instead
     * of 16-bit, which is fine. During LUT preparation, we right-shift each entry
     * by 1, keeping them within the positive range of a signed 16-bit int.
     *
     * For (2), we need to quickly put 'alpha' in the high halfword and invAlpha in
     * the low halfword, or vice versa. One fast way to do this is (0x01000000 + x - (x << 16).
     */

    uint32_t index = arg >> 8;          // Range [0, 0xFF]

    // Load lut[index] into low halfword, lut[index+1] into high halfword.
    uint32_t pair = *(const uint32_t*)(lut + index);

    unsigned alpha = arg & 0xFF;        // Range [0, 0xFF]

    // Reversed halfword order
    uint32_t pairAlpha = (0x01000000 + alpha - (alpha << 16));

    return __SMUADX(pairAlpha, pair) >> 7;
}

void SMLayerFadecandy::fillRefreshRow(uint16_t hardwareY, rgb48 refreshRow[]) {
    const uint8_t *pixelPrev, *pixelNext;
    int i;

    for(i=0; i<this->matrixWidth; i++) {
        pixelPrev = fcbuffers->fbPrev->pixel(i + FRAME_WIDTH * hardwareY);
        pixelNext = fcbuffers->fbNext->pixel(i + FRAME_WIDTH * hardwareY);

        refreshRow[i].red = lutInterpolate(fcbuffers->lutCurrent.r,((pixelPrev[0] * icPrev + pixelNext[0] * icNext) >> 16));
        refreshRow[i].green = lutInterpolate(fcbuffers->lutCurrent.g,((pixelPrev[1] * icPrev + pixelNext[1] * icNext) >> 16));
        refreshRow[i].blue = lutInterpolate(fcbuffers->lutCurrent.b,((pixelPrev[2] * icPrev + pixelNext[2] * icNext) >> 16));

        if(fcbuffers->flags & CFLAG_NO_DITHERING) {
            refreshRow[i].red &= 0xff00;
            refreshRow[i].green &= 0xff00;
            refreshRow[i].blue &= 0xff00;

            // give a visual signal we're in CFLAG_NO_DITHERING (24-bit color) mode
            if(i == 0 && hardwareY == 0) {
                refreshRow[i].red = refreshRow[i].green = refreshRow[i].blue = 0xffff;
            }
        }
    }
}



void SMLayerFadecandy::fillRefreshRow(uint16_t hardwareY, rgb24 refreshRow[]) {
  // no support for 24-bit refreshDepth
}


