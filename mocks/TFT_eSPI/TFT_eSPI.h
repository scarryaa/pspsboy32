#include <stdint.h>

#define TFT_GREEN 0x07E0

class TFT_eSPI
{
public:
    TFT_eSPI();
    void init();
    void fillScreen(uint16_t color);
};