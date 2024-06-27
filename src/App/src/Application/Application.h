#pragma once

#include <memory>
#include <iostream>

#include "MyImgui_SDL2_Opengl3.h"
#include "MyImgui_SDL2_Vulkan.h"

class Application {
public:
    enum class InputHandlers {
        NONE,
        SDL2
    };
    enum class Renders {
        NONE,
        OpenGL3,
        Vulkan
    };

    Application() = default;

    void Start();;
    void SetGuiBackend();
    void Init();
    void Run();
    void RunInner();
    bool IsStartRequired();
    void SetStartRequired(bool value);

private:
    void SetReazation_SDL2(Renders render);

    void CreateSettingWidget();
    void CreateMainWidget();

private:
    std::unique_ptr<Imgui_Interface> m_MyImgui;

    InputHandlers m_CurrentHandler = InputHandlers::NONE;
    Renders m_CurrentRender = Renders::NONE;

    InputHandlers m_HandlerRequired = InputHandlers::NONE;
    Renders m_RenderRequired = Renders::NONE;

    bool m_StartRequired = true;
};