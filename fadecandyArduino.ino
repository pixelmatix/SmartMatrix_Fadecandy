#include "fc_usb.h"
#include "fc_defs.h"
#include <SmartMatrix.h>

static fcBuffers buffers;
fcLinearLUT fcBuffers::lutCurrent;
SmartMatrix matrix;
SMLayerForeground foregroundLayer;
SMLayerBackground backgroundLayer;

extern "C" int usb_rx_handler(usb_packet_t *packet)
{
    // USB packet interrupt handler. Invoked by the ISR dispatch code in usb_dev.c
    return buffers.handleUSB(packet);
}

void setup() {
    matrix.addLayer(&backgroundLayer);
    matrix.addLayer(&foregroundLayer);
    matrix.useDefaultLayers();
    matrix.begin();
    matrix.scrollText("fadecandy",1);

}

void loop() {

}