#pragma once
#include <cstdint>
#include <cmath>

struct Pixel
{
    uint8_t b, g, r, a;
};

struct Ball
{
    float x, y;
    float vx, vy;
    float radius;
    uint32_t color;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void SetPixelData(const uint32_t* pixels, int width, int height);

    // 核心回调
    static void RenderCallback(void* userData, int width, int height, void* pixelBuffer);
    static void UpdateCallback(void* userData, float deltaTime);
    static void KeyCallback(void* userData, int key, bool isPressed);
    static void MouseCallback(void* userData, int x, int y, int button);

private:
    void DrawBall(void* buffer, int width, int height);
    void ClearBuffer(void* buffer, int width, int height);
    void UpdateLogic(float deltaTime);

    uint32_t* m_pixelData;
    Pixel* m_pixelBuffer;
    int m_width;
    int m_height;

    // 动画状态
    Ball m_ball;
    bool m_showBall;

    // 鼠标交互
    int m_mouseX, m_mouseY;
    bool m_mousePressed;
};