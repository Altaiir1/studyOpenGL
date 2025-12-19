//
// Created by Lemi Yürekli on 19.12.25.
//

#include "UI.h"
#include "imgui.h"
#include "glad/glad.h"

void UI::render()
{
    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("Hello, world!");
    ImGui::Checkbox("Set Wireframe Mode", &setWireframe);
    ImGui::End();

    // Apply wireframe mode based on checkbox state
    if (setWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
