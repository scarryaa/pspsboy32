#include "window.hpp"

Window::Window()
{
    this->event = SDL_Event();
    this->window = SDL_CreateWindow("pspsboy32", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (this->window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    SDL_SetWindowResizable(window, SDL_TRUE);

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(this->window);
        SDL_Quit();
    }

    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (this->texture == nullptr)
    {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
}

Window::~Window()
{
    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

void Window::setLoadRomCallback(const std::function<void(const std::string &)> &callback)
{
    this->loadRomCallback = callback;
}

bool Window::init()
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
        return false;
    }

    // Make the window resizable
    SDL_SetWindowResizable(this->window, SDL_TRUE);

    // Initialize SDL Renderer for ImGui
    ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer2_Init(this->renderer);

    return true;
}

void Window::renderDisassembleWindow()
{
}

void Window::renderMemoryWindow()
{
}

void Window::render(uint8_t *frameBuffer)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(this->window);
    ImGui::NewFrame();
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    // this->renderMenuBar();

    // if (this->showDisassembleWindow)
    // {
    //     this->renderDisassembleWindow();
    // }
    // else if (this->showMemoryWindow)
    // {
    //     this->renderMemoryWindow();
    // }

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    this->renderMainWindow();

    ImGui::Render();

    SDL_UpdateTexture(this->texture, NULL, frameBuffer, 160 * sizeof(uint32_t));
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(this->renderer);
}

void Window::renderMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open ROM"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Choose File", ".gb");
                if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::cout << filePathName << std::endl;
                    }
                    ImGuiFileDialog::Instance()->Close();
                }
            }
            if (ImGui::MenuItem("Exit"))
            {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("Disassemble"))
            {
                this->showDisassembleWindow = true;
            }
            if (ImGui::MenuItem("Memory"))
            {
                this->showMemoryWindow = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

SDL_Event Window::getEvent()
{
    return this->event;
}

void Window::handleInput()
{
    ImGui_ImplSDL2_ProcessEvent(&this->event);
    // User requests quit
    while (SDL_PollEvent(&this->event) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&this->event);
        // User requests quit
        if (this->event.type == SDL_QUIT)
        {
            quit();
        }
    }
}

void Window::renderMainWindow()
{
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
                this->loadRomCallback(filePath);
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
        ImGui::Image(this->texture, emulatorSize);

        ImGui::End();
        ImGui::End();
    }
}

void Window::quit()
{
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(0);
}