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

    // Initialize shader after OpenGL context is ready
    try {
        shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
        std::cout << "Shader loaded successfully" << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << '\n';
        return;
    }

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
    
    std::cout << "Setting up VAO, VBO, EBO..." << '\n';
    // Create VAO, VBO, and EBO after OpenGL context is ready
    VAO1 = std::make_unique<VAO>();
    VAO1->Bind();
    
    // Create VBO and EBO as member variables so they stay alive
    VBO1 = std::make_unique<VBO>(vertices, sizeof(vertices));
    EBO1 = std::make_unique<EBO>(indices, sizeof(indices));

    VAO1->LinkVBO(*VBO1, 0);
    // EBO is automatically bound to VAO when created - VAO stores this binding
    // The EBO binding is stored in the VAO, so we can unbind globally
    VBO1->Unbind();  // VBO can be unbound, VAO remembers the configuration
    VAO1->Unbind();   // Unbind VAO, but it remembers the EBO binding
    std::cout << "VAO, VBO, EBO setup complete" << '\n';

    // Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
    
    if (!VAO1 || !VBO1 || !EBO1) {
        std::cerr << "Error: VAO, VBO, or EBO is null, cannot render" << '\n';
        return;
    }
    
    std::cout << "All checks passed, entering render loop" << '\n';
    
    while (!glfwWindowShouldClose(window))
    {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Activate();
        VAO1->Bind();
        // EBO binding is automatically restored when VAO is bound

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
