//
// Created by Lemi Yürekli on 19.12.25.
//

#include <glad/glad.h>
#include <iostream>

#include "imgui.h"

#include "App.h"

#include <cmath>

#include "../include/UI/UI.h"
#include "Shader.h"
#include "../include/BufferObjs/VBO.h"
#include "../include/BufferObjs/VAO.h"
#include "../include/BufferObjs/EBO.h"
#include "Window.h"

// initialized starts at false. Until the initialized is set to true
// any crash will return the initial false boolean.
App::App() : initialized(false), imgui(nullptr)
{
    std::cout << "Constructing App\n\n";

    window = std::make_unique<Window>();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        return;
    }

    imgui = std::make_unique<ImGuiHandler>(window->getGLFWWindow());

    // Initialize shader after OpenGL context is ready
    try {
        std::cout << "Loading Shader successfully\n\n";
        shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
        std::cout << "Shader loaded successfully\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << '\n';
        return;
    }

    // Vertices
    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
   };

    /* unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    }; */
    
    std::cout << "Setting up VAO, VBO, EBO..." << '\n';

    VAO1 = std::make_unique<VAO>();
    VAO1->Bind();
    VBO1 = std::make_unique<VBO>(vertices, sizeof(vertices));
    
    // position attribute (layout = 0)
    VAO1->LinkVBO(*VBO1, 0, 3, 6 * sizeof(float), 0);
    // color attribute (layout = 1)
    VAO1->LinkVBO(*VBO1, 1, 3, 6 * sizeof(float), 3 * sizeof(float));
    
    VBO1->Unbind();
    VAO1->Unbind();

    std::cout << "VAO, VBO, EBO setup complete\n\n";

    // Returns true in the isValid() function so everything went fine
    initialized = true;
}

App::~App()
{
    std::cout << "Terminating App" << '\n';
    // Window destructor will handle glfwTerminate()
}

void App::run()
{
    std::cout << "Starting render loop..." << '\n';
    
    if (!shader) {
        std::cerr << "Error: Shader is null, cannot render" << '\n';
        return;
    }
    
    std::cout << "All checks passed, entering render loop\n" << '\n';
    
    while (!window->shouldClose())
    {
        processInput();
        Clear();

        shader->Activate();

        // Set offset uniform (if you still want to use it)
        GLuint offsetLocation = glGetUniformLocation(shader->ID, "offset");
        glUniform2f(offsetLocation, 0.5f, 0.5f);

        // Set triangle color uniform from settings (which UI modifies)
        // NOTE: Always use 'settings' (the member variable) - don't create new Settings objects!
        GLuint colorLocation = glGetUniformLocation(shader->ID, "triangleColor");
        glUniform3f(colorLocation, settings.triangleColor[0], settings.triangleColor[1], settings.triangleColor[2]);

        VAO1->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // IMGUI - pass settings reference so UI can read/write to it
        imgui->beginFrame();

        ui.render(settings);
        // ImGui::ShowDemoWindow();

        imgui->endFrame();

        window->swapBuffers();
        window->pollEvents();
    }
}

// Returns whether the initialized returns too early (false if any error)
// or not (true)
bool App::isValid() const
{
    return initialized;
}

void App::Clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Here we don't pass window as a parameter since window pointer is a member
// therefore it's part of the Object Data
void App::processInput()
{
    if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->getGLFWWindow(), true);
}
