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
    m_HandlerRequired = InputHandlers::NONE;
    m_RenderRequired = Renders::NONE;
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
    static bool show_demo_window = true;
    static bool show_another_window = false;

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

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);              // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&m_MyImgui->GetClearColor()); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_MyImgui->GetIO().Framerate, m_MyImgui->GetIO().Framerate);
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
}

bool Application::IsStartRequired() {
    return m_StartRequired;
}

void Application::SetStartRequired(bool value) {
    m_StartRequired = value;
}

void Application::SetReazation_SDL2(Renders render) {
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
