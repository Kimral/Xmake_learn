#pragma once

#include <string>
#include <stdexcept>

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "Renders/Opengl3_Render.h"

template<>
class SDL2_InHandler<Opengl3_Render> {
public:
    SDL2_InHandler() 
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            throw std::runtime_error(SDL_GetError());
        }

        // Decide GL+GLSL versions
        #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        m_glsl_version = "#version 100";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        #elif defined(__APPLE__)
        // GL 3.2 Core + GLSL 150
        m_glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        #else
        // GL 3.0 + GLSL 130
        m_glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        #endif

        // From 2.0.18: Enable native IME.
        #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
        #endif

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        m_window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        m_window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, m_window_flags);
        if (m_window == nullptr)
        {
            throw std::runtime_error(SDL_GetError());
        }

        m_gl_context = SDL_GL_CreateContext(m_window);
        SDL_GL_MakeCurrent(m_window, m_gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
    };

    ~SDL2_InHandler() {
        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool PollEvent() {
        return SDL_PollEvent(&m_LastEvent);
    }

    const SDL_Event& GetEvent() {
        return m_LastEvent;
    } 

    SDL_Window* GetWindow() {
        return m_window;
    }

    const SDL_GLContext& GetContext() {
        return m_gl_context;
    }

    const std::string& GetGLSLversion() {
        return m_glsl_version;
    }

    uint32_t GetWindowId(SDL_Window* window) {
        return SDL_GetWindowID(window);
    }

    Opengl3_Render& GetRender() {
        return m_Render;
    }

    void SwapWindows() {
        SDL_GL_SwapWindow(m_window);
    }

private:
    Opengl3_Render m_Render;

    SDL_WindowFlags m_window_flags;
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    SDL_Event m_LastEvent;
    std::string m_glsl_version;
};