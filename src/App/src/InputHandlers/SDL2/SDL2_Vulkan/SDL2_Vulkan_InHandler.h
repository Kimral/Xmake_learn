#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <tuple>

#include <SDL.h>
#include <SDL_vulkan.h>

#include "Vulkan_Render.h"
#include "SDL2_template.h"

template<>
class SDL2_InHandler<Vulkan_Render> {
public:
    SDL2_InHandler();
    ~SDL2_InHandler();

    bool PollEvent() {
        return SDL_PollEvent(&m_LastEvent);
    }

    const SDL_Event& GetEvent() {
        return m_LastEvent;
    }

    uint32_t GetWindowId(SDL_Window* window) {
        return SDL_GetWindowID(m_Window);
    }

    SDL_Window* GetWindow() {
        return m_Window;
    }

    Vulkan_Render& GetRender();

    void CreateWindowSurface();
    std::pair<int, int> GetWindowSize();

private:
    Vulkan_Render m_Render;

    SDL_WindowFlags m_WindowFlags;
    SDL_Window* m_Window;
    SDL_Event m_LastEvent;
};