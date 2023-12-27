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

    int lineOffset = currentScanline % 8 * 2;
    int pixelBaseIndex = currentScanline * SCREEN_WIDTH * 3;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        int tileX = x / 8;
        int tileY = currentScanline / 8;
        int tileMapIndex = tileY * 32 + tileX;

        uint8_t tileNumber = memory.readByte(TILE_MAP_0_BASE_ADDRESS + tileMapIndex);
        uint8_t byte1 = memory.readByte(TILE_DATA_0_BASE_ADDRESS + tileNumber * 16 + lineOffset);
        uint8_t byte2 = memory.readByte(TILE_DATA_0_BASE_ADDRESS + tileNumber * 16 + lineOffset + 1);

        int bitPosition = 7 - (x % 8);
        uint8_t colorBit1 = (byte1 >> bitPosition) & 1;
        uint8_t colorBit2 = (byte2 >> bitPosition) & 1;
        uint8_t colorIndex = (colorBit2 << 1) | colorBit1;

        static const Color colorTable[4] = {WHITE, LIGHT_GRAY, DARK_GRAY, BLACK};
        Color color = colorTable[colorIndex];

        int pixelIndex = pixelBaseIndex + x * 3;
        frameBuffer[pixelIndex] = color.r;
        frameBuffer[pixelIndex + 1] = color.g;
        frameBuffer[pixelIndex + 2] = color.b;
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
