#include <iostream>

#include "App.h"
#include "Timer.h"

int main()
{
    { // Scope ensures timer destructor runs here, printing elapsed time
        utils::Timer timer("Initialization");
    }

    // Initialize Application
    App app;

    if (!app.isValid())
    {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }

    utils::Timer timer("Main loop");
    app.run();

    return 0;
}