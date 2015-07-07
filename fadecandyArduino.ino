// There are issues compiling, if you see "undefined reference" and "error: ld return 1", wait a few seconds then try compiling again
// the order of #includes is very sensitive, including <SmartMatrix.h> before "Layer_Fadecandy.h" will break things for example

#include "Layer_Fadecandy.h"
#include <SmartMatrix.h>

static fcBuffers buffers;
fcLinearLUT fcBuffers::lutCurrent;
SmartMatrix matrix;
SMLayerForeground foregroundLayer;
SMLayerFadecandy fadecandyLayer(&buffers);

extern "C" int usb_rx_handler(usb_packet_t *packet)
{
    // USB packet interrupt handler. Invoked by the ISR dispatch code in usb_dev.c
    return buffers.handleUSB(packet);
}

void setup() {
    matrix.addLayer(&fadecandyLayer);
    matrix.addLayer(&foregroundLayer);
    matrix.begin();
    foregroundLayer.scrollText("fadecandy",1);
}

void loop() {
    // load a single row into the refresh buffer
    //matrix.matrixCalculations();

    // We can switch to the next frame's buffer now.
    buffers.finalizeFrame();

    // Performance counter, for monitoring frame rate externally
    perf_frameCounter++;
}
