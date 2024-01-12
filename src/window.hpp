#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <iostream>
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/ImGuiFileDialog.h"
#include <functional>

class Window
{
    std::function<void(const std::string &)> loadRomCallback;

public:
    void setLoadRomCallback(const std::function<void(const std::string &)> &callback);

    Window();
    ~Window();
    bool showDisassembleWindow = false;
    bool showMemoryWindow = false;

    bool init();
    void render(uint8_t *frameBuffer);
    void handleInput();
    SDL_Event getEvent();
    void quit();
    void finalRender(uint8_t *frameBuffer);

private:
    void renderMenuBar();
    void renderDisassembleWindow();
    void renderMemoryWindow();
    void renderMainWindow();
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Texture *texture;
};

#endif