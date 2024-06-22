#include "Application.h"

int main(int, char**)
{
    Application my_app;
    my_app.SetReazation(std::make_unique<SDL2_Imgui<Opengl3_Render>>());
    my_app.Init();
    my_app.Run();

    return 0;
}