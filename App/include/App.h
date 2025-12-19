//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include <GLFW/glfw3.h>

class App
{
private:
    // Window pointer can be later (in main) be passed as "window")
    GLFWwindow *window;
    bool initialized;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void processInput();

public:
    App();
    ~App();

    void run();
    bool isValid() const;
};
