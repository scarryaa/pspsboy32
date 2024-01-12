#ifdef PLATFORM_ESP32
#include <TFT_eSPI.h>
#include "SD.h"
#include "SPI.h"
#include "esp32-hal-gpio.h"

#define SDCARD_CS 1
#define SDCARD_MOSI 2
#define SDCARD_SCLK 4
#define SDCARD_MISO 6
SPIClass SDSPI(FSPI);

TFT_eSPI tft = TFT_eSPI();
#endif

#include "./file-reader/file-reader.h"
#include <memory>
#include "./logger/logger.h"
#include "./core/core.h"

Core core;
uint8_t lastFrameBuffer[160 * 144 * 2];
bool isLastFrameBufferInitialized = false;

#ifdef PLATFORM_ESP32

void setup()
{
  delay(1000);
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
  Serial.println(SD.exists("roms/LoZ.gb") ? "ROM file exists" : "ROM file does not exist");

  core.init();
  core.loadRom("/roms/Dr Mario (World).gb");
}

bool isFrameBufferChanged(const uint8_t *currentFrameBuffer)
{
  if (!isLastFrameBufferInitialized)
  {
    return true; // Always true for the first frame
  }

  for (int i = 0; i < 160 * 144; ++i)
  {
    if (currentFrameBuffer[i] != lastFrameBuffer[i])
    {
      return true;
    }
  }
  return false;
}

void updateLastFrameBuffer(const uint8_t *currentFrameBuffer)
{
  memcpy(lastFrameBuffer, currentFrameBuffer, sizeof(lastFrameBuffer));
  isLastFrameBufferInitialized = true;
}

void renderGameBoyScreen()
{
  if (core.isFrameReady())
  {
    const uint8_t *currentFrameBuffer = core.getFrameBuffer();

    // Check if the frame buffer has changed
    if (isFrameBufferChanged(currentFrameBuffer))
    {
      // Update TFT display with the frame buffer data
      tft.pushImage(0, 0, 320, 480, const_cast<uint8_t *>(currentFrameBuffer));

      // Update the last frame buffer
      updateLastFrameBuffer(currentFrameBuffer);

      // Reset the frame ready flag in the core
      core.resetFrameReady();
    }
  }
}

void processInput()
{
  // bool aButtonPressed = digitalRead(A_BUTTON_GPIO_PIN);
  // core.setButtonState(Button::A, aButtonPressed);
}

void processAudio()
{
  // Audio processing
}

void loop()
{
  // Execute a single cycle
  core.run();

  // Render Game Boy screen to the TFT display
  renderGameBoyScreen();

  // Read input buttons and process them
  processInput();

  // Process audio
  processAudio();
}
#endif

#ifdef PLATFORM_NATIVE
#include <SDL.h>
#include <iostream>
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/ImGuiFileDialog.h"
#include "emulator.hpp"

auto prev_time = std::chrono::high_resolution_clock::now();

int main(int argc, char *argv[])
{
  Emulator emulator;
  emulator.run();
  return 0;
}
#endif
