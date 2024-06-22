#pragma once

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

class Opengl3_Render {
public:
    void SetViewport(GLint x, GLint y, GLint wigth, GLint heigth) {
        glViewport(x, y, wigth, heigth);
    }
    void SetClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
        glClearColor(red, green, blue, alpha);
    }
    void Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
