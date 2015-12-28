This port of Fadecandy firmware is compatible with the Arduino SmartMatrix 3 Library

Run in combination with the modified Fadecandy server found here:  
https://github.com/pixelmatix/fadecandy/

## Compiling

* Compile with Teensy USB Type: "No USB" - error "multiple definition of `usb_isr'" means you forgot to set USB Type to "No USB"
* Adjust FRAME_WIDTH and FRAME_HEIGHT in fc_defs.h to match kMatrixWidth/Height
* If green artifacts are seen, adjust NUM_USB_BUFFERS in fc_defs.h
