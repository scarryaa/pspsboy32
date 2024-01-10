#ifndef PPU_H
#define PPU_H

#include "../memory/memory.h"
#include <cstdint>
#include <cstring>
#include <cstdbool>
#include <vector>

#define OAM_SEARCH_CYCLES 80
#define PIXEL_TRANSFER_CYCLES 172
#define H_BLANK_CYCLES 204
#define V_BLANK_CYCLES 456
#define V_BLANK_SCANLINE_MAX 153

#define DEBUG_WIDTH 640
#define DEBUG_HEIGHT 480
#define DEBUG_SIZE (DEBUG_WIDTH * DEBUG_HEIGHT)

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define SCX_REGISTER 0xFF43
#define SCY_REGISTER 0xFF42

#define WX_REGISTER 0xFF4B
#define WY_REGISTER 0xFF4A

#define TILE_DATA_0_BASE_ADDRESS 0x8000
#define TILE_DATA_1_BASE_ADDRESS 0x9000
#define TILE_MAP_0_BASE_ADDRESS 0x9800
#define TILE_MAP_1_BASE_ADDRESS 0x9C00
#define OAM_BASE_ADDRESS 0xFE00
#define OAM_SIZE 160

#define STAT 0xFF41
#define LYC_ADDRESS 0xFF45
#define LY_ADDRESS 0xFF44
#define LCDC 0xFF40

#define BGP 0xFF47
#define OBP0 0xFF48
#define OBP1 0xFF49

#define BATCH_SIZE 160
#define OAM_START 0xFE00

struct Pixel
{
    int x, y;
    uint8_t color;
    uint8_t colorIndex;
    uint8_t attributes;
};

extern std::vector<Pixel> pixelBatch;
extern bool debugDrawn;

struct Sprite
{
    int index;
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
    uint8_t *getDebugFrameBuffer();
    bool isFrameReady();
    void resetFrameReady();
    uint8_t readLY();

private:
    uint8_t frameBuffer[SCREEN_SIZE]; // Frame buffer for pixel data
    uint8_t debugFrameBuffer[DEBUG_SIZE];
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
    uint8_t getPaletteColor(uint16_t paletteRegister, uint8_t colorIndex);
    void drawSpritePixel(int x, int y, uint8_t colorIndex, uint8_t color, uint8_t attributes);

    void renderDebug();
    void drawBackground();
    void writeDMA(uint8_t value);
    void flushBatch();
    void flushSpriteBatch(std::vector<Pixel> &batch);

    bool frameProcessed; // Indicates whether the current frame has been processed
    Memory &memory;

    uint8_t tileData[16]; // Buffer to hold tile data for one tile
    uint8_t colorLookupTable[4] = {0xFF, 0xAA, 0x55, 0x00};
    // window internal line counter
    uint8_t win_line_counter;
};

#endif // PPU_H
