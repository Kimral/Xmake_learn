#include "Opengl3_Render.h"

void Opengl3_Render::SetViewport(GLint x, GLint y, GLint wigth, GLint heigth) {
    glViewport(x, y, wigth, heigth);
}

void Opengl3_Render::SetClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    glClearColor(red, green, blue, alpha);
}

void Opengl3_Render::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}
