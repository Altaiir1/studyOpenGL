//
// Created by Lemi Yürekli on 19.12.25.
//

#include <glad/glad.h>
#include <iostream>

#include "App.h"

// initialized starts at false. Until the initialized is set to true
// any crash will return the initial false boolean.
App::App() : window(nullptr), initialized(false)
{
    std::cout << "Constructing App" << '\n';
    // GLFW init
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << '\n';
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        return;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Returns true in the isValid() function so everything went fine
    initialized = true;
}

App::~App()
{
    std::cout << "Terminating App" << '\n';
    glfwTerminate();
}

void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// Returns wether the intialized returns too early (false if any error)
// or not (true)
bool App::isValid() const
{
    return initialized;
}

void App::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Here we don't pass window as a parameter since window pointer is a member
// therefore it's part of the Object Data
void App::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
