#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include "../memory/memory.h"

class PPU
{
public:
    PPU(Memory &memory);
    ~PPU();

    void reset();
    void update(int cycles);

    // Functions to handle different rendering modes
    void handleHBlank();
    void handleVBlank();
    void handleOAMSearch();
    void handlePixelTransfer();

private:
    // PPU registers and flags
    uint8_t LCDC;       // LCD Control Register
    uint8_t STAT;       // LCD Status Register
    uint8_t SCY, SCX;   // Scroll Y and Scroll X
    uint8_t LY;         // LCDC Y-Coordinate
    uint8_t LYC;        // LY Compare
    uint8_t BGP;        // Background Palette
    uint8_t OBP0, OBP1; // Object Palette 0 and 1
    uint8_t WY, WX;     // Window Y and Window X

    int scanlineCounter;

    // Reference to the memory object
    Memory &memory;

    // Frame buffer to store the final rendered image
    uint8_t frameBuffer[160 * 144 * 3];

    // Private methods for internal PPU operations
    void renderScanline();
    void renderTiles();
    void renderSprites();
};

#endif // PPU_H
