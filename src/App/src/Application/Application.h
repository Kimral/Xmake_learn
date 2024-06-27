#pragma once

#include <memory>
#include <iostream>

#include "ApplicationController.h"

class Application {
public:
    Application() = default;

    void Start();
    void Run();

private:
    ApplicationController m_Controller;
};