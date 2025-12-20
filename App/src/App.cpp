//
// Created by Lemi Yürekli on 19.12.25.
//

#include <glad/glad.h>
#include <iostream>

#include "imgui.h"

#include "App.h"
#include "UI.h"
#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

// initialized starts at false. Until the initialized is set to true
// any crash will return the initial false boolean.
App::App() : window(nullptr), initialized(false), imgui(nullptr)
{
    std::cout << "Constructing App\n\n";

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
    float verticesA[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    /* unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    }; */
    
    std::cout << "Setting up VAO, VBO, EBO..." << '\n';

    VAO1 = std::make_unique<VAO>();
    VAO1->Bind();
    VBO1 = std::make_unique<VBO>(verticesA, sizeof(verticesA));
    VAO1->LinkVBO(*VBO1, 0);
    VBO1->Unbind();
    VAO1->Unbind();

    std::cout << "VAO, VBO, EBO setup complete\n\n";

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
    std::cout << "Starting render loop..." << '\n';
    
    if (!shader) {
        std::cerr << "Error: Shader is null, cannot render" << '\n';
        return;
    }
    
    std::cout << "All checks passed, entering render loop\n" << '\n';
    
    while (!glfwWindowShouldClose(window))
    {
        processInput();

        Clear();

        shader->Activate();
        VAO1->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // IMGUI
        imgui->beginFrame();

        ui.render();
        // ImGui::ShowDemoWindow();

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

void App::Clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
