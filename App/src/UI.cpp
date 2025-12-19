//
// Created by Lemi Yürekli on 19.12.25.
//

#include "UI.h"
#include "imgui.h"

void UI::render()
{
    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("Hello, world!");
    ImGui::End();
}
