//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once
#include <GLFW/glfw3.h>


class ImGuiHandler
{
private:

public:
    ImGuiHandler(GLFWwindow* window);
    ~ImGuiHandler();

    void beginFrame();
    void endFrame();
};
