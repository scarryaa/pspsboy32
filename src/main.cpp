#include <TFT_eSPI.h>
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "SD.h"
#include "SPI.h"
#include "esp32-hal-gpio.h"

#define SDCARD_CS 1
#define SDCARD_MOSI 2
#define SDCARD_SCLK 4
#define SDCARD_MISO 6
SPIClass SDSPI(FSPI);

TFT_eSPI tft = TFT_eSPI();
Memory memory;
PPU ppu;
CPU cpu(memory);

void setup()
{
  Serial.begin(115200);
  tft.init();

  // initialize SD card
  pinMode(SDCARD_MISO, INPUT_PULLUP);
  SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
  if (!SD.begin(SDCARD_CS, SDSPI, 4000000, "", 20))
  {
    Serial.println("setup SD card FAILED");
  }
  Serial.println("setup SD card OK");

  // Initialize the emulator components
  Serial.println("setup OK");

  // Read ROM file
  Serial.println("Reading ROM file...");
  Serial.println(SD.exists("roms") ? "roms folder exists" : "roms folder does not exist");

  // Read ROM file into memory
  memory.loadRom();

  // Reset the emulator
  cpu.reset();
}

void renderGameBoyScreen()
{
  tft.fillScreen(TFT_GREEN);
}

void processInput()
{
  // Read the button states and pass them to the emulator
  // Map ESP32 GPIO pin states to Game Boy button states
}

void loop()
{
  // Execute a single CPU cycle
  cpu.executeCycle();

  // Update memory and PPU

  // Render Game Boy screen to the TFT display
  renderGameBoyScreen();

  // Read input buttons and process them
  processInput();
}

void processAudio()
{
  // Audio processing
}

// for native build
int main()
{
  setup();
  while (true)
  {
    loop();
    processAudio();
  }
  return 0;
}
