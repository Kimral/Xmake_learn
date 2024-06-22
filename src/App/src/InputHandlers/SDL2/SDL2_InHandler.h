#pragma once

template <class Render>
class SDL2_InHandler {
public:
    SDL2_InHandler() {};

private:
    Render m_Render;
};

#include "SDL2_Opengl3/SDL2_Opengl3_InHandler.h"