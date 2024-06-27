#include "Application.h"

void Application::Start() {
    m_HandlerRequired = InputHandlers::SDL2;
    m_RenderRequired = Renders::OpenGL3;
    while (IsStartRequired()) {
        SetStartRequired(false);
        try {
            SetGuiBackend();
            Init();
            Run();
        }
        catch (const std::runtime_error& error) {
            std::cout << "Error: " << error.what() << std::endl;
            break;
        }
    }
}

void Application::SetGuiBackend() {
    switch(m_HandlerRequired) {
        case InputHandlers::SDL2: {
            SetReazation_SDL2(m_RenderRequired);
            break;
        }
        default:
            break;
    }
    m_CurrentHandler = m_HandlerRequired;
    m_CurrentRender = m_RenderRequired;
}

void Application::Init() {
    m_MyImgui->SetClearColor(0.45f, 0.55f, 0.60f, 1.00f);
}

void Application::Run() {
    m_MyImgui->Run([this]() {
        this->RunInner();
    });
}

void Application::RunInner() {
    CreateSettingWidget();
    CreateMainWidget();
}

bool Application::IsStartRequired() {
    return m_StartRequired;
}

void Application::SetStartRequired(bool value) {
    m_StartRequired = value;
}

void Application::SetReazation_SDL2(Renders render) {
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

void Application::CreateSettingWidget() {
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Setting");

    static const char* renders[] = {
        "NONE",
        "OpenGL3",
        "Vulkan"
    };

    // Convert the current enum value to an integer for ListBox
    int currentIndex = static_cast<int>(m_RenderRequired);
    if (ImGui::ListBox("##paintkit1", &currentIndex, renders, IM_ARRAYSIZE(renders), 3)) {
        m_RenderRequired = static_cast<Renders>(currentIndex);
    }

    if (ImGui::Button("Restart")) {
        m_StartRequired = true;
        m_MyImgui->Finish();
    }

    ImGui::End();
}

void Application::CreateMainWidget() {
    ImGui::SetNextWindowSize(ImVec2(600, 400));
    ImGui::Begin("Main window");

    int gridSize = 3;
    int buttonWidth = 50;
    int buttonHeight = buttonWidth;
    int spacing = 5;

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize - 1; ++col)
        {
            // Draw button
            std::string buttonLabel = "##";
            ImGui::Button("##", ImVec2(buttonWidth, buttonHeight));
            ImGui::SameLine();
        }
        // Draw button
        std::string buttonLabel = "##";
        ImGui::Button(buttonLabel.c_str(), ImVec2(buttonWidth, buttonHeight));
    }

    ImGui::End();
}
