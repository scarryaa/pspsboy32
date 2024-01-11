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
#include <SDL.h>
#include <iostream>
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/ImGuiFileDialog.h"

auto prev_time = std::chrono::high_resolution_clock::now();

int main(int argc, char *argv[])
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create SDL window
  SDL_Window *window = SDL_CreateWindow("pspsboy32",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        640, 480, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  // Make the window resizable
  SDL_SetWindowResizable(window, SDL_TRUE);

  // Create SDL renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  // Initialize SDL Renderer for ImGui
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  // Initialize the emulator components
  core.init();

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

  while (running)
  {
    // Get input from the user
    const uint8_t *keyboardState = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&e) != 0)
    {
      ImGui_ImplSDL2_ProcessEvent(&e);
      // User requests quit
      if (e.type == SDL_QUIT)
      {
        running = false;
      }
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - prev_time;
    prev_time = currentTime;
    int cyclesToRun = static_cast<int>(elapsed.count() * CPU_FREQUENCY);

    // Start ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    const ImGuiWindowFlags MAIN_WINDOW_FLAGS = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("Main", nullptr, MAIN_WINDOW_FLAGS))
    {
      // Main menu bar
      if (ImGui::BeginMenuBar())
      {
        auto &colors = ImGui::GetStyle().Colors;
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colors[ImGuiCol_TabActive]);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, colors[ImGuiCol_TabActive]);
        ImGui::PushStyleColor(ImGuiCol_Header, colors[ImGuiCol_Tab]);
        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Open ROM"))
          {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".gb");
            ImGui::OpenPopup("File Dialog");
          }
          ImGui::EndMenu();
        }
        ImGui::PopStyleColor(3);
        ImGui::EndMenuBar();
      }

      if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
      {
        // Check if a file was selected
        if (ImGuiFileDialog::Instance()->IsOk())
        {
          // Get the selected file
          std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
          filePath += "\\";
          filePath += ImGuiFileDialog::Instance()->GetCurrentFileName();

          // Load the ROM
          core.reset();
          core.loadRom(filePath);
        }

        // Close the dialog
        ImGuiFileDialog::Instance()->Close();
      }

      // Dock space
      ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

      // Make the emulator window take up the entire dock space
      // Put the emulator window in the dock space and make it take up the entire space

      // Emulator window always on top
      ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Always);
      ImGui::Begin("Emulator", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoSavedSettings);

      // Calculate aspect ratio
      float aspectRatio = 160.0f / 144.0f;

      // Get the available space
      ImVec2 availableSpace = ImGui::GetContentRegionAvail();

      // Calculate the size based on the aspect ratio
      float windowWidth = availableSpace.x;
      float windowHeight = windowWidth / aspectRatio;

      // If the calculated height is more than available, adjust the width
      if (windowHeight > availableSpace.y)
      {
        windowHeight = availableSpace.y;
        windowWidth = windowHeight * aspectRatio;
      }

      // Calculate the horizontal offset for centering
      float offsetX = (availableSpace.x - windowWidth) * 0.6f;

      // Set cursor position to the calculated offset
      ImGui::SetCursorPosX(offsetX);

      // Set the size for the emulator window
      ImVec2 emulatorSize(windowWidth, windowHeight);

      // Render the emulator content
      ImGui::Image(emulatorTexture, emulatorSize);

      ImGui::End();
      ImGui::End();
    }

    // Update emulator core
    bool gameLoaded = core.isGameLoaded();
    while (cyclesToRun > 0 && gameLoaded)
    {
      int cycles = core.run();
      cyclesToRun -= cycles;

      // Update input
      core.setButtonState(Core::Button::Right, keyboardState[SDL_SCANCODE_RIGHT]);
      core.setButtonState(Core::Button::Left, keyboardState[SDL_SCANCODE_LEFT]);
      core.setButtonState(Core::Button::Up, keyboardState[SDL_SCANCODE_UP]);
      core.setButtonState(Core::Button::Down, keyboardState[SDL_SCANCODE_DOWN]);
      core.setButtonState(Core::Button::A, keyboardState[SDL_SCANCODE_Z]);
      core.setButtonState(Core::Button::B, keyboardState[SDL_SCANCODE_X]);
      core.setButtonState(Core::Button::Select, keyboardState[SDL_SCANCODE_A]);
      core.setButtonState(Core::Button::Start, keyboardState[SDL_SCANCODE_S]);
    }

    ImGui::Render();

    // Rendering
    SDL_UpdateTexture(emulatorTexture, nullptr, core.getFrameBuffer(), 160 * sizeof(uint8_t));
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  SDL_DestroyTexture(emulatorTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
#endif
