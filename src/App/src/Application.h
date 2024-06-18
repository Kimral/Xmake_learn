#pragma once

#include <SDL.h>

class Application {
public:
    Application();
    ~Application();

    SDL_Window* GetSdlWindow();

private:
    void Initialize();
    void InitSDL();
    void Finalize();

private:
    SDL_Window* m_pWindow = nullptr;
};