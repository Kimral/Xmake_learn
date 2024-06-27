#include "Application.h"

void Application::Start() {
    while(m_Controller.Restart()) {
        try {
            m_Controller.InitBackend();
            m_Controller.Run();
        }
        catch (const std::runtime_error& error) {
            std::cout << "Error: " << error.what() << std::endl;
            break;
        }
    }
}

void Application::Run() {
    m_Controller.Run();
}



