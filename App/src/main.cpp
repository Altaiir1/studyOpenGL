#include <iostream>

#include "App.h"
#include "utils/Timer.h"

int main()
{

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