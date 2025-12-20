#include "UI.h"

#include <cmath>

#include "imgui.h"
#include "glad/glad.h"

// To create custom  UI components you have to create it inside UI.h
// Create a method and build components below. Lastly: Set the method inside render

// Active or Deactivate UI components
void UI::render()
{
    beginWindow("Settings");
    renderSettings();
    ImGui::Separator();
    textTool();
    endWindow();
}

void UI::beginWindow(const char *title)
{
    ImGui::Begin(title);
}

void UI::endWindow()
{
    ImGui::End();
}

void UI::renderSettings()
{
    ImGui::Text("Wireframe Mode");
    ImGui::Checkbox("Set Wireframe", &setWireframe);

    // Apply wireframe mode based on checkbox state
    if (setWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void UI::textTool()
{
    ImGui::Text("Hello, world %d", 123);
    ImGui::Button("Save");
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);
}
