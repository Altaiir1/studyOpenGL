//
// Created by Lemi Yürekli on 19.12.25.
//

#include <glad/glad.h>
#include <iostream>

#include "imgui.h"

#include "App.h"
#include "UI.h"

// initialized starts at false. Until the initialized is set to true
// any crash will return the initial false boolean.
App::App() : window(nullptr), initialized(false), imgui(nullptr)
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        return;
    }

    imgui = std::make_unique<ImGuiHandler>(window);

    // Vertices
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
   };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Returns true in the isValid() function so everything went fine
    initialized = true;
}

App::~App()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Start ImGui frame
        imgui->beginFrame();

        // ImGui UI
        ui.render();

        ImGui::ShowDemoWindow();

        // Render ImGui
        imgui->endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// Returns whether the initialized returns too early (false if any error)
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
