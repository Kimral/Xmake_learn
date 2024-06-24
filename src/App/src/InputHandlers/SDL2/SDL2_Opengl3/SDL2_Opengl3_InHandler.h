#pragma once

#include <string>
#include <stdexcept>

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "Opengl3_Render.h"
#include "SDL2_template.h"

template<>
class SDL2_InHandler<Opengl3_Render> {
public:
    SDL2_InHandler();
    ~SDL2_InHandler();

    bool PollEvent();
    const SDL_Event& GetEvent();
    SDL_Window* GetWindow();
    const SDL_GLContext& GetContext();
    const std::string& GetGLSLversion();
    uint32_t GetWindowId(SDL_Window* window);
    Opengl3_Render& GetRender();
    void SwapWindows();

private:
    Opengl3_Render m_Render;

    SDL_WindowFlags m_window_flags;
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    SDL_Event m_LastEvent;
    std::string m_glsl_version;
};