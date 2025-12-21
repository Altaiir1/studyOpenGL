#pragma once

#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* window;

public:
    Window();
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    GLFWwindow* getGLFWWindow() const { return window; }
    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    void makeContextCurrent() const;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};