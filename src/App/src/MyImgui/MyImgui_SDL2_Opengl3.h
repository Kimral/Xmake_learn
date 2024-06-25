#pragma once 

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include "MyImgui_interface.h"

#include "SDL2_Opengl3_InHandler.h"

template<>
class MyImgui<SDL2_InHandler<Opengl3_Render>> : public Imgui_Interface {

public:
    MyImgui();
    ~MyImgui() override;

    void Run(std::function<void()> to_Do) override;
    ImGuiIO& GetIO() override;
    void SetClearColor(float r, float g, float b, float a) override;
    ImVec4& GetClearColor() override;

    void Finish() override;

private:
    void NewFrame();
    void ProcessEvent();
    void FinishFrame();

private:
    SDL2_InHandler<Opengl3_Render> m_InputHandler;
    ImVec4 m_clear_color;
};

template<typename Render>
using SDL2_Imgui = MyImgui<SDL2_InHandler<Render>>;