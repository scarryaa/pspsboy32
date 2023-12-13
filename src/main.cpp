#include <TFT_eSPI.h>
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "SD.h"

#define SDCARD_CS 1
#define SDCARD_MOSI 2
#define SDCARD_SCLK 4
#define SDCARD_MISO 6
SPIClass SDSPI(FSPI);

TFT_eSPI tft = TFT_eSPI();
CPU cpu;
Memory memory;
PPU ppu;

void setup() {
  Serial.begin(115200);
  tft.init();

  // initialize SD card
  pinMode(SDCARD_MISO, INPUT_PULLUP);
  SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
  if (!SD.begin(SDCARD_CS, SDSPI)) {
    Serial.println("setup SD card FAILED");
  }
  Serial.println("setup SD card OK");

  // Initialize the emulator components

  // Reset the emulator
  cpu.reset();
  memory.reset();
  Serial.println("setup OK");

  // Read ROM file
  File romFile = SD.open("/roms/04-op r,imm.gb", FILE_READ);
  
  // Read ROM file into memory
  memory.loadRom(romFile);

  romFile.close();
  Serial.println("ROM file read OK");
}

void renderGameBoyScreen() {
  tft.fillScreen(TFT_GREEN);
}

void processInput() {
  // Read the button states and pass them to the emulator
  // Map ESP32 GPIO pin states to Game Boy button states
}

void loop() {
  // Execute a single CPU cycle
  cpu.executeCycle();

  // Update memory and PPU
  
  // Render Game Boy screen to the TFT display
  renderGameBoyScreen();

  // Read input buttons and process them
  processInput();
}

void processAudio() {
  // Audio processing
}
