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
std::unique_ptr<FileReader> fileReader;

#ifdef PLATFORM_ESP32
#include "file-reader/esp-file-reader.h"

void setup()
{
  delay(1000);
  Logger logger;
  fileReader = std::unique_ptr<ESPFileReader>(new ESPFileReader());
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

  // Read ROM file into memory
  if (fileReader->open("/roms/08-misc instrs.gb"))
  {
    char *buffer = new char[0x8000];

    size_t bytesRead = fileReader->read(buffer, 0x8000);
    Serial.println("bytesRead: " + String(bytesRead));
    core.loadRom(buffer, bytesRead);

    fileReader->close();
    delete[] buffer;
  }
  else
  {
    Serial.println("Failed to open ROM file");
  }
}

void renderGameBoyScreen()
{
  // render screen buffer
  if (core.isFrameReady())
  {
    // Update TFT display with the frame buffer data
    tft.pushImage(0, 0, 160, 144, core.getFrameBuffer());

    // Reset the frame ready flag in the core
    core.resetFrameReady();
  }
}

void processInput()
{
  // Read the button states and pass them to the emulator
  // Map ESP32 GPIO pin states to Game Boy button states
}

void processAudio()
{
  // Audio processing
}

void loop()
{
  // Execute a single cycle
  core.update();

  // Render Game Boy screen to the TFT display
  renderGameBoyScreen();

  // Read input buttons and process them
  processInput();

  // Process audio
  processAudio();
}
#endif

#ifdef PLATFORM_NATIVE
#include "file-reader/desktop-file-reader.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create SDL window
  SDL_Window *window = SDL_CreateWindow("pspsboy32",
                                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        640, 480, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  // Create SDL renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  fileReader = std::unique_ptr<DesktopFileReader>(new DesktopFileReader());

  // Initialize the emulator components
  core.init();

  // Reset the emulator
  core.reset();

  std::cout << "Reading ROM file..." << std::endl;

  // Read ROM file into memory
  if (fileReader->open("roms/Dr. Mario (World).gb"))
  {
    char *buffer = new char[0x8000];

    size_t bytesRead = fileReader->read(buffer, 0x8000);
    std::cout << "bytesRead: " << bytesRead << std::endl;
    core.loadRom(buffer, bytesRead);

    fileReader->close();
    delete[] buffer;
  }
  else
  {
    std::cout << "Failed to open ROM file" << std::endl;
  }

  // Main loop
  bool running = true;
  SDL_Event e;
  SDL_Texture *emulatorTexture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGB565,
      SDL_TEXTUREACCESS_STREAMING,
      160, // Frame buffer width
      144  // Frame buffer height
  );

  while (running)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        running = false;
      }
      // Handle other events like input
    }

    // Update emulator core
    core.update();

    if (core.isFrameReady())
    {
      // Update texture with the frame buffer data
      SDL_UpdateTexture(emulatorTexture, nullptr, core.getFrameBuffer(), 160 * 2);

      // Clear screen
      SDL_RenderClear(renderer);

      // Render the emulator screen buffer to the window
      SDL_RenderCopy(renderer, emulatorTexture, nullptr, nullptr);

      // Update screen
      SDL_RenderPresent(renderer);

      // Reset the frame ready flag in the core
      core.resetFrameReady();
    }
  }

  // Cleanup
  SDL_DestroyTexture(emulatorTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
#endif
