#include "ppu.h"

PPU::PPU(Memory &memory) : memory(memory), cycleCounter(0), currentMode(PPUMode::OAMSearch), currentScanline(0)
{
    reset();
}

PPU::~PPU()
{
    // Destructor logic
}

void PPU::reset()
{
    cycleCounter = 0;
    currentMode = PPUMode::OAMSearch;
    currentScanline = 0;
    memset(frameBuffer, 0, sizeof(frameBuffer));
}

void PPU::update(int cycles)
{
    cycleCounter += cycles;

    switch (currentMode)
    {
    case PPUMode::OAMSearch:
        handleOAMSearch();
        break;
    case PPUMode::PixelTransfer:
        handlePixelTransfer();
        break;
    case PPUMode::HBlank:
        handleHBlank();
        break;
    case PPUMode::VBlank:
        handleVBlank();
        break;
    }

    // Render the current scanline
    renderScanline();
    currentScanline++;

    // Reset the scanline if we've reached the end of the screen
    if (currentScanline >= SCANLINES_PER_FRAME)
    {
        currentScanline = 0;
    }

    // Update the PPU mode based on the current scanline
    if (currentScanline < 144)
    {
        if (cycleCounter < 80)
        {
            currentMode = PPUMode::OAMSearch;
        }
        else if (cycleCounter < 252)
        {
            currentMode = PPUMode::PixelTransfer;
        }
        else
        {
            currentMode = PPUMode::HBlank;
        }
    }
    else
    {
        currentMode = PPUMode::VBlank;
    }
}

uint8_t *PPU::getFrameBuffer()
{
    return frameBuffer;
}

Sprite PPU::readSpriteFromOAM(int index)
{
    uint8_t spriteData[4];
    for (int i = 0; i < 4; ++i)
    {
        spriteData[i] = memory.readByte(OAM_BASE_ADDRESS + index * 4 + i);
    }
    Sprite sprite;
    sprite.y = spriteData[0];
    sprite.x = spriteData[1];
    sprite.tileNumber = spriteData[2];
    sprite.attributes = spriteData[3];
    return sprite;
}

bool PPU::isSpriteVisibleOnScanline(Sprite &sprite, int scanline)
{
    return scanline >= sprite.y && scanline < sprite.y + 8;
}

void PPU::handleOAMSearch()
{
    int visibleSprites = 0;
    for (int i = 0; i < 40 && visibleSprites < 10; ++i)
    {
        Sprite sprite = readSpriteFromOAM(i);
        if (isSpriteVisibleOnScanline(sprite, currentScanline))
        {
            memcpy(visibleSpriteData + visibleSprites * 4, &sprite, sizeof(Sprite));
            visibleSprites++;
        }
    }

    // Transition to Pixel Transfer mode after completing OAM Search
    currentMode = PPUMode::PixelTransfer;
}

void PPU::handlePixelTransfer()
{
    // Implement Pixel Transfer logic
    // Transition to HBlank mode after completing Pixel Transfer
}

void PPU::handleHBlank()
{
    // Implement HBlank logic
    // Update scanline and transition to OAMSearch or VBlank mode
}

void PPU::handleVBlank()
{
    // Implement VBlank logic
    // Typically involves setting an interrupt, updating the frame buffer, etc.
}

void PPU::renderScanline()
{
    if (currentScanline >= SCREEN_HEIGHT)
    {
        return;
    }

    uint8_t tileData[16];                                            // Buffer to hold tile data for one tile
    uint16_t colorLookupTable[4] = {0xFFFF, 0xC618, 0x8410, 0x0000}; // Color lookup table

    uint8_t LCDC = memory.readByte(0xFF40);
    bool useTileData1 = (LCDC & 0x10) != 0;
    uint16_t baseDataAddress = useTileData1 ? TILE_DATA_1_BASE_ADDRESS : TILE_DATA_0_BASE_ADDRESS;

    bool useTileMap1 = (LCDC & 0x08) != 0;
    uint16_t baseMapAddress = useTileMap1 ? TILE_MAP_1_BASE_ADDRESS : TILE_MAP_0_BASE_ADDRESS;

    int scrollX = 0;
    int scrollY = 0;

    for (int x = 0; x < SCREEN_WIDTH; x += 8)
    { // Process 8 pixels (one tile) at a time
        int tileX = (x + scrollX) / 8;
        int tileY = (currentScanline + scrollY) / 8;
        int tileMapIndex = tileY * 32 + tileX;
        uint8_t tileNumber = memory.readByte(baseMapAddress + tileMapIndex);

        int16_t signedTileNumber = (int8_t)tileNumber;
        uint16_t tileDataAddress;

        if (useTileData1)
        {
            // When using 8000-8FFF range, tile numbers are unsigned
            tileDataAddress = baseDataAddress + tileNumber * 16;
        }
        else
        {
            // When using 8800-97FF range, tile numbers are signed
            // 0x9000 is the middle of the 8800-97FF range
            tileDataAddress = 0x9000 + signedTileNumber * 16;
        }

        // Read the entire tile data at once
        for (int i = 0; i < 16; ++i)
        {
            tileData[i] = memory.readByte(tileDataAddress + i);
        }

        // Process each pixel in the tile
        for (int tilePixelX = 0; tilePixelX < 8; ++tilePixelX)
        {
            int lineOffset = (currentScanline % 8) * 2;
            uint8_t byte1 = tileData[lineOffset];
            uint8_t byte2 = tileData[lineOffset + 1];
            int bitPosition = 7 - tilePixelX;
            uint8_t colorBit1 = (byte1 >> bitPosition) & 1;
            uint8_t colorBit2 = (byte2 >> bitPosition) & 1;
            uint8_t colorIndex = (colorBit2 << 1) | colorBit1;

            uint16_t colorValue = colorLookupTable[colorIndex];
            int pixelIndex = ((currentScanline * SCREEN_WIDTH) + x + tilePixelX) * 2; // 2 bytes per pixel
            frameBuffer[pixelIndex] = colorValue & 0xFF;                              // Lower byte
            frameBuffer[pixelIndex + 1] = (colorValue >> 8) & 0xFF;                   // Upper byte
        }
    }
}

bool PPU::isFrameReady()
{
    return currentMode == PPUMode::VBlank;
}

void PPU::resetFrameReady()
{
    currentMode = PPUMode::OAMSearch;
}
