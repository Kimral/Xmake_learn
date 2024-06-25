#include "SDL2_Vulkan_InHandler.h"

#include <SDL.h>

#include <vector>
#include <string>

SDL2_InHandler<Vulkan_Render>::SDL2_InHandler() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    // Create window with Vulkan graphics context
    m_WindowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_Window = SDL_CreateWindow("Dear ImGui SDL2 + Vulkan example", 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, m_WindowFlags);
    if (m_Window == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    std::vector<const char*> extensions;
    uint32_t extensions_count = 0;
    SDL_Vulkan_GetInstanceExtensions(m_Window, &extensions_count, nullptr);
    extensions.resize(extensions_count);
    SDL_Vulkan_GetInstanceExtensions(m_Window, &extensions_count, extensions.data());
    GetRender().SetupVulkan(extensions);

    CreateWindowSurface();
};

SDL2_InHandler<Vulkan_Render>::~SDL2_InHandler() {
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

Vulkan_Render& SDL2_InHandler<Vulkan_Render>::GetRender() {
    return m_Render;
}

void SDL2_InHandler<Vulkan_Render>::CreateWindowSurface() {
    if(SDL_Vulkan_CreateSurface(GetWindow(), GetRender().GetInstance(), &GetRender().GetSurface()) == 0) {
        throw std::runtime_error(SDL_GetError());
    }
}

std::pair<int, int> SDL2_InHandler<Vulkan_Render>::GetWindowSize()
{
    int w, h;
    SDL_GetWindowSize(GetWindow(), &w, &h);
    return { w, h };
}

