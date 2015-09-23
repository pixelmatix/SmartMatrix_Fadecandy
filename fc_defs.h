/*
 * Fadecandy Firmware
 *
 * Copyright (c) 2013 Micah Elizabeth Scott
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Definitions for Fadecandy, based on the number of supported LEDs
 */

#pragma once

#define FRAME_WIDTH             64
#define FRAME_HEIGHT            32
#define LEDS_TOTAL              (FRAME_WIDTH * FRAME_HEIGHT)

#define LUT_CH_SIZE             257
#define LUT_TOTAL_SIZE          (LUT_CH_SIZE * 3)

// USB packet layout
#define PIXELS_PER_PACKET       21
#define LUTENTRIES_PER_PACKET   31
#define PACKETS_PER_FRAME       ((LEDS_TOTAL/PIXELS_PER_PACKET) + (LEDS_TOTAL%PIXELS_PER_PACKET ? 1 : 0))
#define PACKETS_PER_LUT         ((LUT_TOTAL_SIZE/LUTENTRIES_PER_PACKET) + (LUT_TOTAL_SIZE%LUTENTRIES_PER_PACKET ? 1 : 0))

// not all values work for this number - if green artifacts are seen in the image, try adding to the "little extra" number one by one until they go away
#define NUM_USB_BUFFERS         ((3*PACKETS_PER_FRAME) + PACKETS_PER_LUT + 4)       // Three full frames, one LUT buffer, a little extra (4)

#define VENDOR_ID               0x1d50    // OpenMoko
#define PRODUCT_ID              0x60bf    // Assigned to Pixelmatix Aurora project
#define DEVICE_VER              0x0107	  // BCD device version
#define DEVICE_VER_STRING		"1.07"
