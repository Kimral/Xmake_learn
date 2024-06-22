#include "Application.h"

#include "MyImguiBackends/MyImgui_SDL2_Opengl3.h"

int main(int, char**)
{
    Application my_app;
    my_app.SetReazation(new SDL2_Imgui<Opengl3_Render>());
    my_app.Init();
    my_app.Run();

    return 0;
}