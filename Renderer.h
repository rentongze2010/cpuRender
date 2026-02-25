#pragma once
#include <cstdint>
#include <cstring>

// RGBA 像素结构 (注意内存布局为 BGRA 以适配 Windows DIB)
struct Pixel
{
    uint8_t b;  // Blue
    uint8_t g;  // Green
    uint8_t r;  // Red
    uint8_t a;  // Alpha
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    // 设置初始像素数据 (ARGB 格式)
    void SetPixelData(const uint32_t* pixels, int width, int height);

    // 核心回调 (供 Window 类调用)
    static void RenderCallback(void* userData, int width, int height, void* pixelBuffer);
    static void UpdateCallback(void* userData, float deltaTime);
    static void KeyCallback(void* userData, int key, bool isPressed);
    static void MouseCallback(void* userData, int x, int y, int button);

private:
    // 内部辅助函数
    void ClearBuffer(void* buffer, int width, int height, uint32_t color = 0xFF202020);
    void UpdateLogic(float deltaTime);

    uint32_t* m_pixelData;      // 用户提供的原始 RGBA 数据
    Pixel* m_pixelBuffer;       // 转换后的 BGRA 缓冲区
    int m_width;
    int m_height;
};