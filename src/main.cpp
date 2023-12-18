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
  Serial.println(SD.exists("roms") ? "roms folder exists" : "roms folder does not exist");

  // Reset the emulator
  core.reset();

  // Read ROM file into memory
  if (fileReader->open("/roms/07-jr,jp,call,ret,rst.gb"))
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
  tft.fillScreen(TFT_GREEN);
}

void processInput()
{
  // Read the button states and pass them to the emulator
  // Map ESP32 GPIO pin states to Game Boy button states
}

void loop()
{
  // Execute a single cycle
  core.update();

  // Render Game Boy screen to the TFT display
  renderGameBoyScreen();

  // Read input buttons and process them
  processInput();
}

void processAudio()
{
  // Audio processing
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
  if (fileReader->open("roms/02-interrupts.gb"))
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

    // Clear screen
    SDL_RenderClear(renderer);

    // Render the emulator screen buffer to the window
    // SDL_RenderCopy(renderer, emulatorTexture, nullptr, nullptr);

    // Update screen
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
#endif
