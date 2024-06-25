#include "MyImgui_SDL2_Opengl3.h"

MyImgui<SDL2_InHandler<Opengl3_Render>>::MyImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_InputHandler.GetWindow(), m_InputHandler.GetContext());
    ImGui_ImplOpenGL3_Init(m_InputHandler.GetGLSLversion().c_str());
}

MyImgui<SDL2_InHandler<Opengl3_Render>>::~MyImgui() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void MyImgui<SDL2_InHandler<Opengl3_Render>>::Run(std::function<void()> to_Do) {
    // Main loop
    #ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    GetIO().IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
    #else
    while (!m_done)
    #endif
    {
        ProcessEvent();
        NewFrame();
        to_Do();
        FinishFrame();
    }
    #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
    #endif
}

ImGuiIO& MyImgui<SDL2_InHandler<Opengl3_Render>>::GetIO() {
    return ImGui::GetIO();
}

void MyImgui<SDL2_InHandler<Opengl3_Render>>::SetClearColor(float r, float g, float b, float a) {
    m_clear_color = ImVec4{ r, g, b, a };
}

ImVec4& MyImgui<SDL2_InHandler<Opengl3_Render>>::GetClearColor() {
    return m_clear_color;
}

void MyImgui<SDL2_InHandler<Opengl3_Render>>::NewFrame() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MyImgui<SDL2_InHandler<Opengl3_Render>>::ProcessEvent() {
    while (m_InputHandler.PollEvent())
    {
        const SDL_Event& l_Event = m_InputHandler.GetEvent();
        ImGui_ImplSDL2_ProcessEvent(&l_Event);
        if (l_Event.type == SDL_QUIT)
            m_done = true;
        if (l_Event.type == SDL_WINDOWEVENT &&
            l_Event.window.event == SDL_WINDOWEVENT_CLOSE &&
            l_Event.window.windowID == m_InputHandler.GetWindowId(m_InputHandler.GetWindow()))
            m_done = true;
    }
}

void MyImgui<SDL2_InHandler<Opengl3_Render>>::FinishFrame() {
    // Rendering
    ImGui::Render();

    m_InputHandler.GetRender().SetViewport(0, 0, (int)GetIO().DisplaySize.x, (int)GetIO().DisplaySize.y);
    m_InputHandler.GetRender().SetClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w, m_clear_color.z * m_clear_color.w, m_clear_color.w);
    m_InputHandler.GetRender().Clear();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_InputHandler.SwapWindows();
}
