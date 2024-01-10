#include "ppu.h"

std::vector<Pixel> pixelBatch;
bool debugDrawn = false;

PPU::PPU(Memory &memory) : memory(memory)
{
    reset();
    pixelBatch = std::vector<Pixel>();

    memset(debugFrameBuffer, 0xFF, DEBUG_SIZE);
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
            renderScanline();
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

            // increment the window line counter if the window is enabled
            if (memory.readByte(LCDC) & 0x20)
            {
                memory.writeByte(WY_REGISTER, memory.readByte(WY_REGISTER) + 1);
            }

            memory.writeByte(LY_ADDRESS, currentScanline); // Update LY

            if (currentScanline == SCREEN_HEIGHT)
            {
                currentMode = PPUMode::VBlank;
                updateSTATInterrupt(); // Update STAT at the start of VBlank
            }
            else
            {
                currentMode = PPUMode::OAMSearch;
                updateSTATInterrupt(); // Update STAT at the start of OAM Search
            }
        }
        break;
    case PPUMode::VBlank:
        memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x01);          // Set VBlank interrupt flag
        memory.writeByte(0xFF41, (memory.readByte(0xFF41) & 0xFC) | 0x01); // Set mode flag to VBlank
        // reset the window line counter
        memory.writeByte(WY_REGISTER, 0);

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
}

void PPU::flushBatch()
{
    for (const auto &pixel : pixelBatch)
    {
        drawPixel(frameBuffer, pixel.x, pixel.y, pixel.color);
    }
    pixelBatch.clear();
}

void PPU::flushSpriteBatch(std::vector<Pixel> &batch)
{
    for (const auto &pixel : batch)
    {
        drawSpritePixel(pixel.x, pixel.y, pixel.colorIndex, pixel.color, pixel.attributes);
    }
    batch.clear();
}

uint8_t PPU::readLY()
{
    return memory.readByte(LY_ADDRESS);
}

uint8_t *PPU::getDebugFrameBuffer()
{
    return debugFrameBuffer;
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

    // Render window
    renderWindow();

    // Render sprites
    renderSprites();

    // Render debug once
    if (debugDrawn)
        return;
    else
    {
        debugDrawn = true;
        renderDebug();
    }
}

bool PPU::isSpriteVisible(Sprite sprite)
{
    uint8_t _LCDC = memory.readByte(LCDC);

    // Check if sprites are enabled
    if (!(_LCDC & 0x02))
        return false;

    // Check if the sprite is on the current scanline
    if (sprite.y > currentScanline || sprite.y + 8 <= currentScanline)
        return false;

    return true;
}

uint8_t PPU::getSpritePixelColor(Sprite sprite, int x, int y)
{
    uint8_t _LCDC = memory.readByte(LCDC);

    // Check if sprites are enabled
    if (!(_LCDC & 0x02))
        return 0xFF;

    if (sprite.attributes & 0x20)
        x = 7 - x;
    if (sprite.attributes & 0x40)
        y = 7 - y;

    bool is8x16 = (_LCDC & 0x04) >> 1;
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

    return colorIndex;
}

uint8_t PPU::getPaletteColor(uint16_t paletteRegister, uint8_t colorIndex)
{
    uint8_t palette = memory.readByte(paletteRegister);
    uint8_t colorBits = (palette >> (colorIndex * 2)) & 0x03;

    switch (colorBits)
    {
    case 0:
        return 0xFF;
    case 1:
        return 0xB6;
    case 2:
        return 0x6D;
    case 3:
        return 0x00;
    default:
        return 0xFF;
    }
}

void PPU::renderSprites()
{
    if (!(memory.readByte(LCDC) & 0x02))
        return;

    uint8_t spriteHeight = 8;
    bool is8x16 = memory.readByte(LCDC) & 0x04;
    if (is8x16)
    {
        spriteHeight = 16;
    }

    uint8_t spriteXBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    std::fill_n(spriteXBuffer, SCREEN_WIDTH * SCREEN_HEIGHT, 0xFF);

    // Draw a sprite
    int spritesDrawnOnLine = 0;
    for (int i = 0; i < 40; i++)
    {
        if (spritesDrawnOnLine >= 10)
        {
            break; // Stop processing after 10 sprites have been drawn on the current scanline
        }

        Sprite sprite;
        sprite.y = memory.readByte(OAM_START + i * 4) - 16;
        sprite.x = memory.readByte(OAM_START + i * 4 + 1) - 8;
        sprite.tileNumber = memory.readByte(OAM_START + i * 4 + 2);
        sprite.attributes = memory.readByte(OAM_START + i * 4 + 3);

        if (!isSpriteVisible(sprite))
            continue;

        spritesDrawnOnLine++;

        std::vector<Pixel> spriteBatch;

        // Draw a scanline of the sprite
        for (int y = 0; y < spriteHeight; y++)
        {
            for (int x = 0; x < 8; x++)
            {

                // Check for BG/Window Priority
                bool bgWinPriority = sprite.attributes & 0x80;
                if (bgWinPriority)
                    continue;

                int screenX = sprite.x + x;
                int screenY = sprite.y + y;

                // Check for sprite y-flip
                if (sprite.attributes & 0x40)
                {
                    screenY = sprite.y + spriteHeight - y - 1;
                }

                // Check for sprite x-flip
                if (sprite.attributes & 0x20)
                {
                    screenX = sprite.x + 7 - x;
                }

                // Check for sprite-to-sprite priority
                if (sprite.x < spriteXBuffer[screenY * SCREEN_WIDTH + screenX])
                {
                    if (screenX < 0 || screenX >= SCREEN_WIDTH || screenY < 0 || screenY >= SCREEN_HEIGHT)
                        continue;

                    uint8_t colorIndex = getSpritePixelColor(sprite, x, y);
                    uint8_t color = getPaletteColor((sprite.attributes & 0x10) ? OBP1 : OBP0, colorIndex);

                    spriteBatch.push_back({sprite.x + x, sprite.y + y, color, colorIndex, sprite.attributes});

                    spriteXBuffer[screenY * SCREEN_WIDTH + screenX] = sprite.x;
                }

                // Flush the batch if it's full
                if (spriteBatch.size() == BATCH_SIZE)
                {
                    flushSpriteBatch(spriteBatch);
                }
            }
        }

        flushSpriteBatch(spriteBatch);
    }
}

void PPU::renderDebug()
{
    for (int x = 0; x < DEBUG_WIDTH; x++)
    {
        for (int y = 0; y < DEBUG_HEIGHT; y++)
        {
            drawPixel(debugFrameBuffer, x, y, 0xFF);
        }
    }
}

void PPU::renderWindow()
{
    // Check LCDC.5 to see if window is enabled
    if (!(memory.readByte(LCDC) & 0x20) || !(memory.readByte(LCDC) & 0x01))
        return;

    // Check if the window is visible on the current scanline
    if (currentScanline < memory.readByte(WY_REGISTER))
        return;

    bool addrMode = memory.readByte(0xFF40) & 0x10;
    uint16_t baseAddr = addrMode ? TILE_DATA_0_BASE_ADDRESS : TILE_DATA_1_BASE_ADDRESS;

    bool useTileMap1 = memory.readByte(LCDC) & 0x40;
    uint16_t baseMapAddress = useTileMap1 ? TILE_MAP_1_BASE_ADDRESS : TILE_MAP_0_BASE_ADDRESS;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        uint8_t wx = memory.readByte(WX_REGISTER);
        uint8_t wy = memory.readByte(WY_REGISTER);

        if (x < wx - 7)
            continue;

        int adjustedX = x - (wx - 7);
        int adjustedY = currentScanline - wy;

        uint8_t tileX = adjustedX / 8;
        uint8_t tileY = adjustedY / 8;

        uint16_t tileMapAddress = baseMapAddress + tileY * 32 + tileX;
        uint8_t tileIndex = memory.readByte(tileMapAddress);

        uint16_t tileAddress;
        if (addrMode)
        {
            tileAddress = baseAddr + tileIndex * 16;
        }
        else
        {
            int8_t signedTileIndex = static_cast<int8_t>(tileIndex);
            tileAddress = 0x9000 + signedTileIndex * 16;
        }

        uint8_t colorIndex = getTilePixelColor(tileAddress, adjustedX % 8, adjustedY % 8);
        uint8_t color = getPaletteColor(BGP, colorIndex);

        // check if we are in bounds
        if (x < 0 || x >= SCREEN_WIDTH || currentScanline < 0 || currentScanline >= SCREEN_HEIGHT)
            continue;

        pixelBatch.push_back({x, currentScanline, color});

        // Flush the batch if it's full
        if (pixelBatch.size() == BATCH_SIZE)
        {
            flushBatch();
        }
    }

    flushBatch();
}

uint8_t PPU::getTilePixelColor(uint16_t address, uint8_t x, uint8_t y)
{
    uint8_t lo = memory.readByte(address + y * 2);
    uint8_t hi = memory.readByte(address + y * 2 + 1);

    uint8_t bitIndex = 7 - x;
    uint8_t loBit = (lo >> bitIndex) & 0x01;
    uint8_t hiBit = (hi >> bitIndex) & 0x01;
    uint8_t colorIndex = (hiBit << 1) | loBit;
    return colorIndex;
}

void PPU::renderBackground()
{
    if (!(memory.readByte(LCDC) & 0x01))
        return;

    uint8_t scx = memory.readByte(SCX_REGISTER);
    uint8_t scy = memory.readByte(SCY_REGISTER);

    bool addrMode = memory.readByte(0xFF40) & 0x10;
    uint16_t baseAddr = addrMode ? TILE_DATA_0_BASE_ADDRESS : TILE_DATA_1_BASE_ADDRESS;

    bool useTileMap1 = !(memory.readByte(LCDC) & 0x08);
    uint16_t baseMapAddress = useTileMap1 ? TILE_MAP_0_BASE_ADDRESS : TILE_MAP_1_BASE_ADDRESS;

    int y = currentScanline; // Use the current scanline
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        int adjustedX = (x + scx) % 256;
        int adjustedY = (y + scy) % 256;

        uint8_t tileX = adjustedX / 8;
        uint8_t tileY = adjustedY / 8;

        uint16_t tileMapAddress = baseMapAddress + tileY * 32 + tileX;
        uint8_t tileIndex = memory.readByte(tileMapAddress);

        uint16_t tileAddress;
        if (addrMode)
        {
            tileAddress = baseAddr + tileIndex * 16;
        }
        else
        {
            int8_t signedTileIndex = static_cast<int8_t>(tileIndex);
            tileAddress = 0x9000 + signedTileIndex * 16;
        }

        uint8_t colorIndex = getTilePixelColor(tileAddress, adjustedX % 8, adjustedY % 8);
        uint8_t color = getPaletteColor(BGP, colorIndex);

        // check if we are in bounds
        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
            continue;

        pixelBatch.push_back({x, y, color});

        // Flush the batch if it's full
        if (pixelBatch.size() == BATCH_SIZE)
        {
            flushBatch();
        }
    }

    flushBatch();
}

void PPU::drawPixel(uint8_t *frameBuffer, int x, int y, uint8_t color)
{
    uint32_t index = (y * SCREEN_WIDTH + x);
    frameBuffer[index] = color;
}

void PPU::drawSpritePixel(int x, int y, uint8_t colorIndex, uint8_t color, uint8_t attributes)
{
    // Skip transparent pixels (color index 0)
    if (colorIndex == 0)
        return;

    drawPixel(frameBuffer, x, y, color);
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
    uint8_t _STAT = memory.readByte(STAT);
    uint8_t LY = memory.readByte(0xFF44);
    uint8_t LYC = memory.readByte(0xFF45);
    bool interruptRequested = false;

    // Check LYC=LY coincidence
    if ((LY == LYC) && (_STAT & 0x40))
    {
        interruptRequested = true;
    }

    // Check for mode-specific STAT interrupts
    if (((currentMode == PPUMode::OAMSearch) && (_STAT & 0x20)) ||
        ((currentMode == PPUMode::VBlank) && (_STAT & 0x10)) ||
        ((currentMode == PPUMode::HBlank) && (_STAT & 0x08)))
    {
        interruptRequested = true;
    }

    if (interruptRequested)
    {
        // Set STAT interrupt flag in interrupt flag register
        memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x02);
    }
}