#include "core/core.h"
#include "window.hpp"

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void loadRom(const std::string &filePath);
    void reset();
    void run();
    void handleInput(const SDL_Event &event);

    bool isGameLoaded() const;

private:
    Core core;
    Window window;
    bool gameLoaded;
    bool paused;
};