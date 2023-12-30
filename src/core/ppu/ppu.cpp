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

    // Render background
    for (int i = 0; i < SCREEN_WIDTH; ++i)
    {
        // Calculate the tile number for this pixel
        int tileNumber = (i + (currentScanline * SCREEN_WIDTH)) / 8;

        // Calculate the tile address
        int tileAddress = TILE_DATA_0_BASE_ADDRESS + memory.readByte(TILE_MAP_0_BASE_ADDRESS + tileNumber);

        // Read the tile data into the buffer
        for (int j = 0; j < 16; ++j)
        {
            tileData[j] = memory.readByte(tileAddress + j);
        }

        // Calculate the offset into the tile data
        int tileDataOffset = (i % 8) + ((currentScanline % 8) * 2);

        // Calculate the color index for this pixel
        int colorIndex = ((tileData[tileDataOffset + 1] >> (7 - (i % 8))) & 0x01) << 1;
        colorIndex |= (tileData[tileDataOffset] >> (7 - (i % 8))) & 0x01;

        // Calculate the offset into the frame buffer
        int frameBufferOffset = i + (currentScanline * SCREEN_WIDTH);

        // Set the pixel color in the frame buffer
        frameBuffer[frameBufferOffset] = colorLookupTable[colorIndex];
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
