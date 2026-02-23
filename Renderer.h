#pragma once
#include <vector>
#include <cstdint>

// RGBA 像素结构 (注意内存布局)
struct Pixel
{
    uint8_t b;  // Windows BMP 格式是 BGRA
    uint8_t g;
    uint8_t r;
    uint8_t a;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    // 设置像素数据 (RGBA 格式，每个像素 4 字节)
    void SetPixelData(const uint32_t* pixels, int width, int height);

    // 获取像素缓冲区指针 (用于直接渲染)
    void* GetPixelBuffer() { return m_pixelBuffer; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // 渲染回调函数 (供 Window 调用)
    static void RenderCallback(void* userData, int width, int height, void* pixelBuffer);

private:
    uint32_t* m_pixelData;      // 用户提供的 RGBA 数据
    Pixel* m_pixelBuffer;       // 渲染用的 BGRA 缓冲区
    int m_width;
    int m_height;
};