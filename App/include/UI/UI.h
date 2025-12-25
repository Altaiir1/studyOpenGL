#pragma once

class Settings;

class UI
{
public:
    void render(Settings& settings);

private:
    void beginWindow(const char* title);
    void endWindow();

    void renderSettings(Settings& settings);
    // void renderTriangleColor(Settings& settings);
    void windowColor(Settings& settings);

    void textTool();
};
