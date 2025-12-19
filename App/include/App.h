//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include <GLFW/glfw3.h>

#include "ImGuiHandler.h"

class App
{
private:
    // Window pointer can be later (in main) be passed as "window")
    GLFWwindow *window;
    bool initialized;
    // We make a pointer to imgui, since (in App.cpp) we Member initialize window
    // with nullptr first and then imgui -> therefore imgui can't initialize window
    // Can't pass nullptr to a non-pointer initialization
    std::unique_ptr<ImGuiHandler> imgui;

    static void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);

    void processInput();

public:
    App();

    ~App();

    void run();

    bool isValid() const;
};
