#ifndef _LAYER_FADECANDY_H_
#define _LAYER_FADECANDY_H_

#include <Layer.h>
#include <MatrixCommon.h>
#include "fc_usb.h"

class SMLayerFadecandy : public SM_Layer {
    public:
        SMLayerFadecandy(fcBuffers * newfcbuffers, uint16_t width, uint16_t height);
        void frameRefreshCallback();
        void fillRefreshRow(uint16_t hardwareY, rgb48 refreshRow[]);
        void fillRefreshRow(uint16_t hardwareY, rgb24 refreshRow[]);

    private:
        inline uint32_t calculateFcInterpCoefficient();
        fcBuffers * fcbuffers;
};

#endif