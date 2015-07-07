#ifndef _LAYER_FADECANDY_H_
#define _LAYER_FADECANDY_H_

#include "fc_usb.h"
#include <Layer.h>
#include "MatrixCommon.h"

class SMLayerFadecandy : public SM_Layer {
    public:
        SMLayerFadecandy(fcBuffers * newfcbuffers);
        void frameRefreshCallback();
        void getRefreshPixel(uint8_t x, uint8_t y, rgb24 &refreshPixel);
        void getRefreshPixel(uint8_t x, uint8_t y, rgb48 &refreshPixel);

    private:
        inline uint32_t calculateFcInterpCoefficient();
        fcBuffers * fcbuffers;
};

#endif