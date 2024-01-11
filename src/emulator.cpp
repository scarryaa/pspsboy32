#include "emulator.hpp"

Emulator::Emulator()
{
    gameLoaded = false;
    paused = false;
    window.setLoadRomCallback([this](const std::string &filePath)
                              { loadRom(filePath); });
}

Emulator::~Emulator()
{
}

void Emulator::run()
{
    auto prev_time = std::chrono::high_resolution_clock::now();
    window.init();
    core.init();

    bool running = true;
    while (running)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - prev_time;
        prev_time = currentTime;
        int cyclesToRun = static_cast<int>(elapsed.count() * CPU_FREQUENCY);

        window.handleInput();
        while (cyclesToRun > 0 && gameLoaded && !paused)
        {
            int cycles = core.run();
            cyclesToRun -= cycles;

            handleInput(window.getEvent());
        }
        window.render(core.getFrameBuffer());
    }
}

void Emulator::loadRom(const std::string &filePath)
{
    printf("Loading ROM: %s\n", filePath.c_str());
    if (core.loadRom(filePath))
    {
        core.reset();
        gameLoaded = true;
    }
}

void Emulator::reset()
{
    core.reset();
}

void Emulator::handleInput(const SDL_Event &event)
{
    const uint8_t *keyboardState = SDL_GetKeyboardState(nullptr);
    window.handleInput();
    core.setButtonState(Core::Button::Right, keyboardState[SDL_SCANCODE_RIGHT]);
    core.setButtonState(Core::Button::Left, keyboardState[SDL_SCANCODE_LEFT]);
    core.setButtonState(Core::Button::Up, keyboardState[SDL_SCANCODE_UP]);
    core.setButtonState(Core::Button::Down, keyboardState[SDL_SCANCODE_DOWN]);
    core.setButtonState(Core::Button::A, keyboardState[SDL_SCANCODE_Z]);
    core.setButtonState(Core::Button::B, keyboardState[SDL_SCANCODE_X]);
    core.setButtonState(Core::Button::Select, keyboardState[SDL_SCANCODE_A]);
    core.setButtonState(Core::Button::Start, keyboardState[SDL_SCANCODE_S]);
}