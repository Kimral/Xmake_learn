#pragma once

#include <memory>

#include "MyImgui_SDL2_Opengl3.h"
#include "MyImgui_SDL2_Vulkan.h"

class BackendSystem {
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

    static const InputHandlers k_DefaultHandler = InputHandlers::SDL2;
    static const Renders k_DefaultRender = Renders::OpenGL3;

    void Run(std::function<void()> function);
    void Init();
    void RequestInputHandler(InputHandlers handler);
    void RequestRender(Renders render);
    void Finish();

    InputHandlers GetCurrentHandler();
    Renders GetCurrentRender();

private:
    void InitBackend(InputHandlers inputHandler, Renders render);
    void SetReazation_SDL2(Renders render);

private:
	std::unique_ptr<Imgui_Interface> m_MyImgui;

    InputHandlers m_CurrentHandler = InputHandlers::NONE;
    Renders m_CurrentRender = Renders::NONE;

    InputHandlers m_HandlerRequested = InputHandlers::NONE;
    Renders m_RenderRequested = Renders::NONE;
};