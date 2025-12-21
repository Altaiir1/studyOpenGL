#include <cmath>

#include "imgui.h"
#include "glad/glad.h"

#include "UI/UI.h"
#include "UI/Settings.h"

void UI::render(Settings& settings)
{
    beginWindow("Settings");
    renderSettings(settings);
    ImGui::Separator();
    textTool();
    endWindow();

    /* beginWindow("Triangle");
    renderTriangleColor(settings);
    endWindow(); */
}

void UI::beginWindow(const char *title)
{
    ImGui::Begin(title);
}

void UI::endWindow()
{
    ImGui::End();
}

void UI::renderSettings(Settings& settings)
{
    ImGui::Text("Wireframe Mode");
    // ImGui directly modifies settings.wireframeMode
    ImGui::Checkbox("Set Wireframe", &settings.wireframeMode);

    // Apply wireframe mode based on settings state
    if (settings.wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*
void UI::renderTriangleColor(Settings& settings)
{
    ImGui::Text("Triangle Color");
    ImGui::ColorEdit3("Color", settings.triangleColor);
}
*/

void UI::textTool()
{
    ImGui::Text("Hello, world %d", 123);
    ImGui::Button("Save");
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);
}
