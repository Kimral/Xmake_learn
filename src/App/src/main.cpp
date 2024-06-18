#include "Application.h"

// Main code
int main(int, char**)
{
    Application app;
    app.Init();
    app.Run();
    app.Shutdown();
    return 0;
}