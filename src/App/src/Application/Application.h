#pragma once

#include <memory>

#include "MyImgui_SDL2_Opengl3.h"

class Application {
public:
    enum class InputHandlers {
        SDL2
    };
    enum class Renders {
        OpenGL3
    };

    Application() = default;

    void SetGuiBackend(InputHandlers inputHandler, Renders render);
    void Init();
    void Run();
    void RunInner();

private:
    void SetReazation_SDL2(Renders render);

private:
    std::unique_ptr<Imgui_Interface> m_MyImgui;
    InputHandlers m_InputHandler;
    Renders m_Render;
};