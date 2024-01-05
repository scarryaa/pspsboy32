#include "ppu.h"

PPU::PPU(Memory &memory) : memory(memory)
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
        if (cycleCounter >= OAM_SEARCH_CYCLES)
        {
            cycleCounter -= OAM_SEARCH_CYCLES;
            currentMode = PPUMode::PixelTransfer;
            updateSTATInterrupt(); // Update STAT at the start of Pixel Transfer
        }
        break;
    case PPUMode::PixelTransfer:
        if (cycleCounter >= PIXEL_TRANSFER_CYCLES)
        {
            cycleCounter -= PIXEL_TRANSFER_CYCLES;
            currentMode = PPUMode::HBlank;
            updateSTATInterrupt(); // Update STAT at the start of HBlank
        }
        break;
    case PPUMode::HBlank:
        if (cycleCounter >= H_BLANK_CYCLES)
        {
            cycleCounter -= H_BLANK_CYCLES;
            currentScanline++;
            memory.writeByte(LY_ADDRESS, currentScanline); // Update LY

            if (currentScanline == SCREEN_HEIGHT)
            {
                currentMode = PPUMode::VBlank;
                memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x01); // Set VBlank interrupt flag
                updateSTATInterrupt();                                    // Update STAT at the start of VBlank
            }
            else
            {
                currentMode = PPUMode::OAMSearch;
                updateSTATInterrupt(); // Update STAT at the start of OAM Search
            }
        }
        break;
    case PPUMode::VBlank:
        if (cycleCounter >= V_BLANK_CYCLES)
        {
            cycleCounter -= V_BLANK_CYCLES;
            currentScanline++;
            if (currentScanline > V_BLANK_SCANLINE_MAX)
            {
                currentScanline = 0;
                currentMode = PPUMode::OAMSearch;
                updateSTATInterrupt(); // Update STAT at the start of OAM Search
                frameProcessed = false;
            }
            memory.writeByte(LY_ADDRESS, currentScanline); // Update LY
        }
        break;
    }

    renderScanline();
}

uint8_t PPU::readLY()
{
    return memory.readByte(LY_ADDRESS);
}

uint8_t *PPU::getFrameBuffer()
{
    return frameBuffer;
}

void PPU::renderScanline()
{
    if (currentScanline >= SCREEN_HEIGHT)
    {
        return;
    }

    // Render background
    renderBackground();

    // Render sprites
    renderSprites();

    // Render window
    renderWindow();
}

void PPU::renderSprites()
{
}

void PPU::renderWindow()
{
}

void PPU::renderBackground()
{
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
    if (currentMode == PPUMode::VBlank && !frameProcessed)
    {
        frameProcessed = true;
        return true;
    }
    return false;
}

void PPU::resetFrameReady()
{
    frameProcessed = false;
}

void PPU::updateSTATInterrupt()
{
    uint8_t STAT = memory.readByte(0xFF41);
    uint8_t LY = memory.readByte(0xFF44);
    uint8_t LYC = memory.readByte(0xFF45);
    bool interruptRequested = false;

    // Check LYC=LY coincidence
    if ((LY == LYC) && (STAT & 0x40))
    {
        interruptRequested = true;
    }

    // Check Mode 2 OAM interrupt
    if ((currentMode == PPUMode::OAMSearch) && (STAT & 0x20))
    {
        interruptRequested = true;
    }

    // Check Mode 1 VBlank interrupt
    if ((currentMode == PPUMode::VBlank) && (STAT & 0x10))
    {
        interruptRequested = true;
    }

    // Check Mode 0 HBlank interrupt
    if ((currentMode == PPUMode::HBlank) && (STAT & 0x08))
    {
        interruptRequested = true;
    }

    if (interruptRequested)
    {
        // Set STAT interrupt flag in interrupt flag register
        memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x02);
    }
}
