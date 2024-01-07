#ifndef PPU_H
#define PPU_H

#include "../memory/memory.h"
#include <cstdint>
#include <cstring>
#include <cstdbool>

#define OAM_SEARCH_CYCLES 80
#define PIXEL_TRANSFER_CYCLES 172
#define H_BLANK_CYCLES 204
#define V_BLANK_CYCLES 4560
#define V_BLANK_SCANLINE_MAX 153

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define SCX_REGISTER 0xFF43
#define SCY_REGISTER 0xFF42

#define TILE_DATA_0_BASE_ADDRESS 0x8000
#define TILE_DATA_1_BASE_ADDRESS 0x8800
#define TILE_MAP_0_BASE_ADDRESS 0x9800
#define TILE_MAP_1_BASE_ADDRESS 0x9C00
#define OAM_BASE_ADDRESS 0xFE00
#define OAM_SIZE 160

#define LY_ADDRESS 0xFF44
#define LCDC 0xFF40

struct Sprite
{
    uint8_t y;
    uint8_t x;
    uint8_t tileNumber;
    uint8_t attributes;
};

struct Color
{
    uint16_t value;
};

// Define the PPU modes
enum class PPUMode
{
    OAMSearch,
    PixelTransfer,
    HBlank,
    VBlank
};

class PPU
{
public:
    PPU(Memory &memory);
    ~PPU();

    // Resets the PPU to its initial state
    void reset();

    // Updates the PPU state based on the number of CPU cycles that have elapsed
    void update(int cycles);

    // Gets a pointer to the frame buffer
    uint8_t *getFrameBuffer();
    bool isFrameReady();
    void resetFrameReady();
    uint8_t readLY();

private:
    uint8_t frameBuffer[SCREEN_SIZE]; // Frame buffer for pixel data
    int cycleCounter;                 // Counts the cycles to determine the PPU's current state
    PPUMode currentMode;              // Current mode of the PPU
    int currentScanline;              // Current scanline being processed
    Sprite visibleSpriteData[10 * 4]; // Stores the sprite data for the visible sprites

    // Renders a single scanline
    void renderScanline();
    void renderSprites();
    void renderWindow();
    void renderBackground();
    void updateSTATInterrupt();
    void drawPixel(uint8_t *frameBuffer, int x, int y, uint8_t color);
    uint8_t getTilePixelColor(uint16_t address, uint8_t x, uint8_t y);
    bool isSpriteVisible(Sprite sprite);
    uint8_t getSpritePixelColor(Sprite sprite, int x, int y);

    void drawBackground();
    void writeDMA(uint8_t value);

    bool frameProcessed; // Indicates whether the current frame has been processed
    Memory &memory;

    uint8_t tileData[16]; // Buffer to hold tile data for one tile
    uint8_t colorLookupTable[4] = {0xFF, 0xB6, 0x6D, 0x00};
};

#endif // PPU_H
