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
    // bool lcdEnabled = memory.readByte(LCDC) & 0x80 != 0;

    // if (!lcdEnabled)
    // {
    //     // LCD is disabled, so reset the PPU state
    //     // set bits 0-1 of STAT to 0
    //     reset();
    //     memory.writeByte(0xFF41, memory.readByte(0xFF41) & 0xFC);
    //     return;
    // }

    cycleCounter += cycles;

    switch (currentMode)
    {
    case PPUMode::OAMSearch:
        memory.writeByte(0xFF41, (memory.readByte(0xFF41) & 0xFC) | 0x02); // Set mode flag to OAM Search
        if (cycleCounter >= OAM_SEARCH_CYCLES)
        {
            cycleCounter -= OAM_SEARCH_CYCLES;
            currentMode = PPUMode::PixelTransfer;
            updateSTATInterrupt(); // Update STAT at the start of Pixel Transfer
        }
        break;
    case PPUMode::PixelTransfer:
        memory.writeByte(0xFF41, (memory.readByte(0xFF41) & 0xFC) | 0x03); // Set mode flag to Pixel Transfer
        if (cycleCounter >= PIXEL_TRANSFER_CYCLES)
        {
            cycleCounter -= PIXEL_TRANSFER_CYCLES;
            currentMode = PPUMode::HBlank;
            updateSTATInterrupt(); // Update STAT at the start of HBlank
        }
        break;
    case PPUMode::HBlank:
        memory.writeByte(0xFF41, (memory.readByte(0xFF41) & 0xFC) | 0x00); // Set mode flag to HBlank
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
        memory.writeByte(0xFF41, (memory.readByte(0xFF41) & 0xFC) | 0x01); // Set mode flag to VBlank
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

bool PPU::isSpriteVisible(Sprite sprite)
{
    int sx = sprite.x - 8;
    int sy = sprite.y - 16;

    int spriteWidth = 8;
    int spriteHeight = 8; // or 16, if using 8x16 mode

    bool isWithinXBounds = (sx + spriteWidth > 0) && (sx < SCREEN_WIDTH);
    bool isWithinYBounds = (sy + spriteHeight > 0) && (sy < SCREEN_HEIGHT);

    return isWithinXBounds && isWithinYBounds;
}

uint8_t PPU::getSpritePixelColor(Sprite sprite, int x, int y)
{
    uint8_t _LCDC = memory.readByte(LCDC);
    bool is8x16 = (_LCDC & 0x02) >> 1;
    uint8_t tileIndex = sprite.tileNumber;
    if (is8x16)
    {
        // Adjust tile index for the second tile of 8x16 sprite
        if (y >= 8)
        {
            tileIndex += 1;
            y -= 8; // Adjust y to the second tile
        }
    }

    uint16_t rowAddr = TILE_DATA_0_BASE_ADDRESS + tileIndex * 16 + y * 2;
    uint8_t lo = memory.readByte(rowAddr);
    uint8_t hi = memory.readByte(rowAddr + 1);

    uint8_t bitIndex = 7 - x;
    uint8_t loBit = (lo >> bitIndex) & 0x01;
    uint8_t hiBit = (hi >> bitIndex) & 0x01;
    uint8_t colorIndex = (hiBit << 1) | loBit;
    return colorLookupTable[colorIndex];
}

void PPU::renderSprites()
{
    uint8_t spriteCount = 0;
    uint16_t BASE_ADDR = OAM_BASE_ADDRESS;
    uint8_t SPRITE_DATA_SIZE = 4;

    // Load sprite data
    for (int i = 0; i < 40; i++)
    {
        uint16_t spriteAddr = BASE_ADDR + (i * SPRITE_DATA_SIZE);

        Sprite tmp;
        tmp.y = memory.readByte(spriteAddr);
        tmp.x = memory.readByte(spriteAddr + 1);
        tmp.tileNumber = memory.readByte(spriteAddr + 2);
        tmp.attributes = memory.readByte(spriteAddr + 3);

        visibleSpriteData[i] = tmp;
    }

    uint8_t SPRITE_HEIGHT = memory.readByte(LCDC) & 0x02 >> 1 ? 16 : 8;
    uint8_t SPRITE_WIDTH = 8;

    // Render sprites
    for (Sprite &sprite : visibleSpriteData)
    {
        if (isSpriteVisible(sprite))
        {
            for (int sx = 0; sx < SPRITE_WIDTH; sx++)
            {
                for (int sy = 0; sy < SPRITE_HEIGHT; sy++)
                {
                    uint8_t colorIndex = getSpritePixelColor(sprite, sx, sy);

                    if (colorIndex == 0)
                        continue;
                    uint8_t color = colorLookupTable[colorIndex];
                    drawPixel(frameBuffer, sprite.x + sx - 8, sprite.y + sy - 16, color);
                }
            }
        }
    }
}

void PPU::renderWindow()
{
}

uint8_t PPU::getTilePixelColor(uint16_t address, uint8_t x, uint8_t y)
{
    uint16_t rowAddr = address + y * 2;
    uint8_t lo = memory.readByte(rowAddr);
    uint8_t hi = memory.readByte(rowAddr + 1);

    uint8_t bitIndex = 7 - x;

    uint8_t loBit = (lo >> bitIndex) & 0x01;
    uint8_t hiBit = (hi >> bitIndex) & 0x01;
    uint8_t colorIndex = (hiBit << 1) | loBit;
    return colorLookupTable[colorIndex];
}

void PPU::renderBackground()
{
    uint8_t scx = memory.readByte(SCX_REGISTER);
    uint8_t scy = memory.readByte(SCY_REGISTER);

    bool addrMode = memory.readByte(0xFF40) & 0x10;
    uint16_t baseAddr = memory.readByte(0xFF40) & 0x10 ? TILE_DATA_0_BASE_ADDRESS : TILE_DATA_1_BASE_ADDRESS;

    bool useTileMap1 = memory.readByte(LCDC) & 0x08 != 0;
    uint16_t baseMapAddress = useTileMap1 ? TILE_MAP_0_BASE_ADDRESS : TILE_MAP_1_BASE_ADDRESS;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            uint8_t tileX = (scx + x) / 8;
            uint8_t tileY = (scy + y) / 8;

            uint16_t tileId = memory.readByte(baseMapAddress + tileY * 32 + tileX);
            uint16_t tileAddr = baseAddr + (addrMode ? tileId * 16 : ((int8_t)tileId + 128) * 16);
            uint8_t color = getTilePixelColor(tileAddr, x % 8, y % 8);
            drawPixel(frameBuffer, x, y, color);
        }
    }
}

void PPU::drawPixel(uint8_t *frameBuffer, int x, int y, uint8_t color)
{
    uint32_t index = (y * SCREEN_WIDTH + x);
    frameBuffer[index] = color;
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
