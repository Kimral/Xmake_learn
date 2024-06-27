#include "BackendSystem.h"

void BackendSystem::Run(std::function<void()> function) {
    m_MyImgui->Run(function);
}

void BackendSystem::Init() {
    InitBackend(m_HandlerRequested, m_RenderRequested);
    if(m_MyImgui.get() == nullptr) {
        InitBackend(k_DefaultHandler, k_DefaultRender);
    }
    m_MyImgui->SetClearColor(0.45f, 0.55f, 0.60f, 1.00f);
}

void BackendSystem::InitBackend(InputHandlers inputHandler, Renders render) {
    switch (inputHandler) {
        case InputHandlers::SDL2: {
            SetReazation_SDL2(render);
            break;
        }
        default:
            m_MyImgui.reset();
            break;
    }
    m_CurrentHandler = inputHandler;
    m_CurrentRender = render;
}

void BackendSystem::SetReazation_SDL2(Renders render) {
    m_MyImgui.reset();
    switch (render) {
        case Renders::OpenGL3: {
            m_MyImgui = std::make_unique<SDL2_Imgui<Opengl3_Render>>();
            break;
        }
        case Renders::Vulkan: {
            m_MyImgui = std::make_unique<SDL2_Imgui<Vulkan_Render>>();
            break;
        }
    }
}

void BackendSystem::RequestRender(Renders render) {
    m_RenderRequested = render;
}

void BackendSystem::RequestInputHandler(InputHandlers handler) {
    m_HandlerRequested = handler;
}

void BackendSystem::Finish() {
    m_MyImgui->Finish();
}

BackendSystem::InputHandlers BackendSystem::GetCurrentHandler() {
    return m_CurrentHandler;
}

BackendSystem::Renders BackendSystem::GetCurrentRender() {
    return m_CurrentRender;;
}
