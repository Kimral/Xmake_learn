#include <iostream>

#include "Application.h"

int main(int, char**)
{
    Application my_app;
    try {
        my_app.SetGuiBackend(Application::InputHandlers::SDL2, Application::Renders::Vulkan);
        my_app.Init();
        my_app.Run();
    }
    catch(const std::runtime_error& error) {
        std::cout << "Error: " << error.what() << std::endl;
    }
    return 0;
}