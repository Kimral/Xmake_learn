#pragma once

#include <functional>

#include "imgui.h"

template <class InputHandler>
class MyImgui {
    MyImgui() {}   
};

class Imgui_Interface {
public:
    Imgui_Interface() {};
    virtual ~Imgui_Interface() = default;

    virtual void Run(std::function<void()> to_Do) = 0;
    virtual ImGuiIO& GetIO() = 0;
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    virtual ImVec4& GetClearColor() = 0;
    virtual void Finish() = 0;

    bool& Done() {
        return m_Done;
    }

private:
    bool m_Done = false;
};

