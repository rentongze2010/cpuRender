#include "Renderer.h"

Renderer::Renderer()
    : m_pixelData(nullptr), m_pixelBuffer(nullptr),
    m_width(0), m_height(0)
{
}

Renderer::~Renderer()
{
    delete[] m_pixelBuffer;
    // m_pixelData 通常是外部传入的指针，不由 Renderer 释放
}

void Renderer::SetPixelData(const uint32_t* pixels, int width, int height)
{
    m_width = width;
    m_height = height;

    // 重新分配缓冲区
    delete[] m_pixelBuffer;
    m_pixelBuffer = new Pixel[width * height];
    m_pixelData = (uint32_t*)pixels;

    // 转换 RGBA 到 BGRA (Windows DIB 格式要求)
    // 格式：0xAABBGGRR (小端序)
    for (int i = 0; i < width * height; i++)
    {
        uint32_t rgba = pixels[i];
        m_pixelBuffer[i].a = (rgba >> 24) & 0xFF;
        m_pixelBuffer[i].r = (rgba >> 16) & 0xFF;
        m_pixelBuffer[i].g = (rgba >> 8) & 0xFF;
        m_pixelBuffer[i].b = rgba & 0xFF;
    }
}

void Renderer::ClearBuffer(void* buffer, int width, int height, uint32_t color)
{
    Pixel* pixels = (Pixel*)buffer;
    Pixel fillPixel;
    fillPixel.a = (color >> 24) & 0xFF;
    fillPixel.r = (color >> 16) & 0xFF;
    fillPixel.g = (color >> 8) & 0xFF;
    fillPixel.b = color & 0xFF;

    // 快速填充
    for (int i = 0; i < width * height; i++)
    {
        pixels[i] = fillPixel;
    }
}

void Renderer::UpdateCallback(void* userData, float deltaTime)
{
    Renderer* p = (Renderer*)userData;
    if (p) p->UpdateLogic(deltaTime);
}

void Renderer::UpdateLogic(float deltaTime)
{
    // 预留位置：在这里添加你的动画逻辑
    // 例如：修改 m_pixelData 中的像素值实现动态效果
    // 当前为空，仅显示静态图像
}

void Renderer::RenderCallback(void* userData, int width, int height, void* pixelBuffer)
{
    Renderer* p = (Renderer*)userData;
    if (!p) return;

    Pixel* dest = (Pixel*)pixelBuffer;

    // 1. 清空屏幕 (深灰色背景)
    p->ClearBuffer(dest, width, height, 0xff000000);

    // 2. 绘制用户提供的图像 (如果尺寸匹配)
    if (p->m_pixelBuffer && p->m_width > 0 && p->m_height > 0)
    {
        int copyWidth = (width < p->m_width) ? width : p->m_width;
        int copyHeight = (height < p->m_height) ? height : p->m_height;

        // 将内部缓冲区拷贝到屏幕缓冲区
        for (int y = 0; y < copyHeight; y++)
        {
            memcpy(dest + y * width, p->m_pixelBuffer + y * p->m_width, copyWidth * sizeof(Pixel));
        }
    }
}

void Renderer::KeyCallback(void* userData, int key, bool isPressed)
{
    // 预留位置：处理键盘事件
    // 例如：按 ESC 退出，按空格暂停等
    (void)userData; (void)key; (void)isPressed; // 避免未使用警告
}

void Renderer::MouseCallback(void* userData, int x, int y, int button)
{
    // 预留位置：处理鼠标事件
    // 例如：点击交互，画笔工具等
    (void)userData; (void)x; (void)y; (void)button; // 避免未使用警告
}