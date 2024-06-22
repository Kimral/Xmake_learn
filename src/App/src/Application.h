#pragma once

#include "InputHandlers.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

template <class InputHandler, class Render>
class MyImgui {
    MyImgui() {}   
};

template<>
class MyImgui<SDL2_InHandler<Opengl3_Render>, Opengl3_Render> {

public:
    MyImgui() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        
        ImGui::CreateContext();
        ImGuiIO& io = GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(m_InputHandler.GetWindow(), m_InputHandler.GetContext());
        ImGui_ImplOpenGL3_Init(m_InputHandler.GetGLSLversion().c_str());
    }

    void Run() {
        static bool show_demo_window = true;
        static bool show_another_window = false;
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        bool done = false;
        #ifdef __EMSCRIPTEN__
        // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
        // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
        io.IniFilename = nullptr;
        EMSCRIPTEN_MAINLOOP_BEGIN
        #else
        while (!done)
        #endif
        {
            while (m_InputHandler.PollEvent())
            {
                const SDL_Event& l_Event = m_InputHandler.GetEvent();
                ImGui_ImplSDL2_ProcessEvent(&l_Event);
                if (l_Event.type == SDL_QUIT)
                    done = true;
                if (l_Event.type == SDL_WINDOWEVENT && 
                    l_Event.window.event == SDL_WINDOWEVENT_CLOSE && 
                    l_Event.window.windowID == m_InputHandler.GetWindowId(m_InputHandler.GetWindow()))
                    done = true;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GetIO().Framerate, GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                show_another_window = false;
                ImGui::End();
            }

            // Rendering
            ImGui::Render();

            m_InputHandler.GetRender().SetViewport(0, 0, (int)GetIO().DisplaySize.x, (int)GetIO().DisplaySize.y);
            m_InputHandler.GetRender().SetClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            m_InputHandler.GetRender().Clear();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            m_InputHandler.SwapWindows();
        }
        #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_MAINLOOP_END;
        #endif
    }

    ~MyImgui() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    ImGuiIO& GetIO() {
        return ImGui::GetIO();
    }

private:
    SDL2_InHandler<Opengl3_Render> m_InputHandler;
};

class Application {
    
public:
    Application() = default;

    void Run() {
        m_MyImgui.Run();
    }

private:
    MyImgui<SDL2_InHandler<Opengl3_Render>, Opengl3_Render> m_MyImgui;
};