#include "UI.h"
#include "imgui.h"
#include "glad/glad.h"

// To create custom  UI components you have to create it inside UI.h
// Create a method and build components below. Lastly: Set the method inside render

// Active or Deactivate UI components
void UI::render()
{
    renderSettings();
}

void UI::renderSettings()
{
    ImGui::Begin("Settings");

    ImGui::Text("Wireframe Mode");
    ImGui::Checkbox("Set Wireframe", &setWireframe);

    ImGui::End();

    // Apply wireframe mode based on checkbox state
    if (setWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}