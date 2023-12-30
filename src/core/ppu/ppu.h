#ifndef PPU_H
#define PPU_H

#include "../memory/memory.h"
#include <cstdint>
#include <cstring>

// Define the screen dimensions and other constants
constexpr int SCREEN_WIDTH = 160;
constexpr int SCREEN_HEIGHT = 144;
constexpr int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
constexpr int CYCLES_PER_SCANLINE = 456;
constexpr int SCANLINES_PER_FRAME = 154;
constexpr int CYCLES_PER_FRAME = CYCLES_PER_SCANLINE * SCANLINES_PER_FRAME;

constexpr int TILE_WIDTH = 8;
constexpr int TILE_HEIGHT = 8;
constexpr int TILE_SIZE = TILE_WIDTH * TILE_HEIGHT;

constexpr int TILE_MAP_WIDTH = 32;
constexpr int TILE_MAP_HEIGHT = 32;
constexpr int TILE_MAP_SIZE = TILE_MAP_WIDTH * TILE_MAP_HEIGHT;

constexpr int TILE_DATA_0_BASE_ADDRESS = 0x8000;
constexpr int TILE_DATA_1_BASE_ADDRESS = 0x8800;
constexpr int TILE_DATA_2_BASE_ADDRESS = 0x9000;
constexpr int TILE_DATA_3_BASE_ADDRESS = 0x9800;

constexpr int TILE_MAP_0_BASE_ADDRESS = 0x9800;
constexpr int TILE_MAP_1_BASE_ADDRESS = 0x9C00;

constexpr int OAM_BASE_ADDRESS = 0xFE00;

struct Sprite
{
    uint8_t y;
    uint8_t x;
    uint8_t tileNumber;
    uint8_t flags;
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

private:
    Memory &memory;                    // Reference to the memory
    uint8_t frameBuffer[SCREEN_SIZE];  // Frame buffer for pixel data
    int cycleCounter;                  // Counts the cycles to determine the PPU's current state
    PPUMode currentMode;               // Current mode of the PPU
    int currentScanline;               // Current scanline being processed
    uint8_t visibleSpriteData[10 * 4]; // Stores the sprite data for the visible sprites

    // Handles the OAM search phase
    void handleOAMSearch();

    // Handles the pixel transfer phase
    void handlePixelTransfer();

    // Handles the HBlank phase
    void handleHBlank();

    // Handles the VBlank phase
    void handleVBlank();

    Color getColorFromIndex(uint8_t index);

    // Renders a single tile
    void renderTile(uint8_t *frameBuffer, int tileX, int tileY, uint8_t *tileData);

    void renderBackground();

    // Renders a single scanline
    void renderScanline();

    // Renders the frame
    void renderFrame();

    Sprite readSpriteFromOAM(int index);
    bool isSpriteVisibleOnScanline(Sprite &sprite, int scanline);
};

#endif // PPU_H
