#pragma once

class UI
{
public:
    void render();

private:
    void beginWindow(const char* title);
    void endWindow();

    // Components
    void renderSettings();
    bool setWireframe = false;

    void textTool();
};
