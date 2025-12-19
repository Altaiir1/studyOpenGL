//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuiHandler.h"
#include "UI.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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
    UI ui;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VAO> VAO1;
    std::unique_ptr<VBO> VBO1;
    std::unique_ptr<EBO> EBO1;

    static void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);
    void processInput();

public:
    App();
    ~App();

    void run();
    bool isValid() const;
    static void Clear() ;
};
