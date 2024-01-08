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

uint8_t previousFrameBuffer[160 * 144] = {0};
Core core;
std::unique_ptr<FileReader> fileReader;
uint8_t lastFrameBuffer[160 * 144 * 2];
bool isLastFrameBufferInitialized = false;

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
  if (fileReader->open("/roms/Dr. Mario (World).gb"))
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

void IRAM_ATTR renderGameBoyScreen()
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

  // Create Debug window
  SDL_Window *debugWindow = SDL_CreateWindow("Debug",
                                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                             640, 480, SDL_WINDOW_SHOWN);

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

  // Create SDL debug renderer
  SDL_Renderer *debugRenderer = SDL_CreateRenderer(debugWindow, -1, SDL_RENDERER_ACCELERATED);
  if (debugRenderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(debugWindow);
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
  if (fileReader->open("C:/Users/Ficis/OneDrive/Desktop/pspsboy32/roms/Dr. Mario (World).gb"))
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
      SDL_PIXELFORMAT_RGB332,
      SDL_TEXTUREACCESS_STREAMING,
      160, // Frame buffer width
      144  // Frame buffer height
  );

  SDL_Texture *debugTexture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGB332,
      SDL_TEXTUREACCESS_STREAMING,
      320, // Frame buffer width
      480  // Frame buffer height
  );

  while (running)
  {
    // Get input from the user
    const uint8_t *keyboardState = SDL_GetKeyboardState(nullptr);

    while (SDL_PollEvent(&e) != 0)
    {
      // User requests quit
      if (e.type == SDL_QUIT)
      {
        running = false;
      }
    }

    // Update input
    core.setButtonState(Core::Button::Right, keyboardState[SDL_SCANCODE_RIGHT]);
    core.setButtonState(Core::Button::Left, keyboardState[SDL_SCANCODE_LEFT]);
    core.setButtonState(Core::Button::Up, keyboardState[SDL_SCANCODE_UP]);
    core.setButtonState(Core::Button::Down, keyboardState[SDL_SCANCODE_DOWN]);
    core.setButtonState(Core::Button::A, keyboardState[SDL_SCANCODE_Z]);
    core.setButtonState(Core::Button::B, keyboardState[SDL_SCANCODE_X]);
    core.setButtonState(Core::Button::Select, keyboardState[SDL_SCANCODE_A]);
    core.setButtonState(Core::Button::Start, keyboardState[SDL_SCANCODE_S]);

    // Update emulator core
    core.update();

    if (core.isFrameReady())
    {
      if (memcmp(previousFrameBuffer, core.getFrameBuffer(), sizeof(previousFrameBuffer)) == 0)
      {
        // Framebuffers are the same, skip rendering
        continue;
      }

      // Framebuffers are not the same, update previousFrameBuffer and render
      memcpy(previousFrameBuffer, core.getFrameBuffer(), sizeof(previousFrameBuffer));

      SDL_UpdateTexture(emulatorTexture, nullptr, core.getFrameBuffer(), 160 * sizeof(uint8_t));
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, emulatorTexture, nullptr, nullptr);
      SDL_RenderPresent(renderer);

      // Render debug window
      SDL_UpdateTexture(debugTexture, nullptr, core.getDebugFrameBuffer(), 160 * sizeof(uint8_t));
      SDL_RenderClear(debugRenderer);
      SDL_RenderCopy(debugRenderer, debugTexture, nullptr, nullptr);
      SDL_RenderPresent(debugRenderer);

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
