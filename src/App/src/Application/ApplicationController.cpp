#include "ApplicationController.h"

#include <string>

#include "imgui.h"

void ApplicationController::RequireRestart() {
    m_Restart = true;
}

bool ApplicationController::Restart()
{
    bool restart = m_Restart;
    m_Restart = false;
    return restart;
}

void ApplicationController::Run() {
    m_Backend.Run([&]() {
        CreateSettingWidget();
        CreateMainWidget();
    });
}


void ApplicationController::InitBackend() {
    m_Backend.Init();
}

void ApplicationController::CreateSettingWidget() {
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Setting");

    static const char* renders[] = {
        "NONE",
        "OpenGL3",
        "Vulkan"
    };

    // Convert the current enum value to an integer for ListBox
    if (ImGui::ListBox("##paintkit1", &m_SelectedRender, renders, IM_ARRAYSIZE(renders), 3)) {
        m_Backend.RequestRender(static_cast<BackendSystem::Renders>(m_SelectedRender));
        m_Backend.RequestInputHandler(m_Backend.GetCurrentHandler());
    }

    if (ImGui::Button("Restart")) {
        RequireRestart();
        m_Backend.Finish();
    }

    ImGui::End();
}

void ApplicationController::CreateMainWidget() {
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
