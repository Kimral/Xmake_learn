#pragma once

#include <string>
#include <stdexcept>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <vector>

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
        return SDL_GetWindowID(m_window);
    }


    SDL_Window* GetWindow() {
        return m_window;
    }

    Vulkan_Render& GetRender();

private:
    Vulkan_Render m_Render;

    SDL_WindowFlags m_window_flags;
    SDL_Window* m_window;
    SDL_Event m_LastEvent;
};