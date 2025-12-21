#pragma once

#include <memory>
#include <UI/Settings.h>

#include "Window.h"
#include "ImGuiHandler.h"
#include "UI/UI.h"
#include "Shader.h"
#include "BufferObjs/VAO.h"
#include "BufferObjs/VBO.h"
#include "BufferObjs/EBO.h"
#include "Textures/Texture.h"

struct AppSettings
{

};

class App
{
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<ImGuiHandler> imgui;
    UI ui;
    bool initialized;

    // Rendering resources
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture1;
    std::unique_ptr<Texture> texture2;
    std::unique_ptr<VAO> VAO1;
    std::unique_ptr<VBO> VBO1;
    std::unique_ptr<EBO> EBO1;

    // ImGui Settings
    Settings settings;

    // Initialization methogs
    bool initWindow();
    bool initOpenGL();
    bool initImGui();
    bool initShaders();
    bool initTextures();
    bool initGeometry();

    // Rendering methods
    void setupShaderUniforms();
    void render();
    void renderUI();
    void Clear();

    // Input
    void processInput();

public:
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();
    bool isValid() const;
};
