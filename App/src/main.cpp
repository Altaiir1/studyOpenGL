#include <iostream>

#include "App.h"

int main()
{
    // Initialize Application
    App app;

    if (!app.isValid())
    {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }

    // Run main loop
    app.run();

    return 0;
}