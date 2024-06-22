#include "Application.h"

int main(int, char**)
{
    Application my_app;
    my_app.SetGuiBackend(Application::InputHandlers::SDL2, Application::Renders::OpenGL3);
    my_app.Init();
    my_app.Run();
    return 0;
}