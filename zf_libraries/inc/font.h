#ifndef font___
#define font___

#include <stdint.h>

typedef enum
{
    RGB565_WHITE    = (0xFFFF),
    RGB565_BLACK    = (0x0000),
    RGB565_BLUE     = (0x001F),
    RGB565_PURPLE   = (0xF81F),
    RGB565_PINK     = (0xFE19),
    RGB565_RED      = (0xF800),
    RGB565_MAGENTA  = (0xF81F),
    RGB565_GREEN    = (0x07E0),
    RGB565_CYAN     = (0x07FF),
    RGB565_YELLOW   = (0xFFE0),
    RGB565_BROWN    = (0xBC40),
    RGB565_GRAY     = (0x8430),

    RGB565_39C5BB   = (0x3616),
    RGB565_66CCFF   = (0x665F),
}rgb565_color_enum;

extern const uint8_t      ascii_font_6x8[][6];

#endif
