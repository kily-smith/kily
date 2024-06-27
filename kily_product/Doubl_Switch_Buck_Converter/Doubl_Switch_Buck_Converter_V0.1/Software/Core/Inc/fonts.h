#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint32_t *data;
} FontDef;

extern const FontDef Font_7x10;
extern const FontDef Font_11x18;
extern const FontDef Font_8x10;
extern const FontDef Font_Custom;

extern const unsigned char IMG_DATA[25600] ;
extern const unsigned char gImage_12[1420];
extern const unsigned char gImage_123[21920];
extern const unsigned char IMG_DATA1[14400];
extern const uint8_t  IMG[100*100*2];
#endif
