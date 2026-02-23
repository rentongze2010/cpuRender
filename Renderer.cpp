#include "Renderer.h"
#include <cstring>

Renderer::Renderer()
    : m_pixelData(nullptr), m_pixelBuffer(nullptr),
    m_width(0), m_height(0)
{
}

Renderer::~Renderer()
{
    delete[] m_pixelBuffer;
}

void Renderer::SetPixelData(const uint32_t* pixels, int width, int height)
{
    m_width = width;
    m_height = height;

    // 重新分配缓冲区
    delete[] m_pixelBuffer;
    m_pixelBuffer = new Pixel[width * height];

    // 保存用户数据指针
    m_pixelData = (uint32_t*)pixels;

    // 转换 RGBA 到 BGRA (Windows DIB 格式)
    for (int i = 0; i < width * height; i++)
    {
        uint32_t rgba = pixels[i];
        m_pixelBuffer[i].r = (rgba >> 24) & 0xFF;
        m_pixelBuffer[i].g = (rgba >> 16) & 0xFF;
        m_pixelBuffer[i].b = (rgba >> 8) & 0xFF;
        m_pixelBuffer[i].a = rgba & 0xFF;
    }
}

void Renderer::RenderCallback(void* userData, int width, int height, void* pixelBuffer)
{
    Renderer* pRenderer = (Renderer*)userData;

    if (!pRenderer || !pRenderer->m_pixelBuffer)
        return;

    // 将预渲染的像素数据拷贝到屏幕缓冲区
    // 注意：这里需要考虑缩放或裁剪
    int copyWidth = (width < pRenderer->m_width) ? width : pRenderer->m_width;
    int copyHeight = (height < pRenderer->m_height) ? height : pRenderer->m_height;

    Pixel* dest = (Pixel*)pixelBuffer;
    Pixel* src = pRenderer->m_pixelBuffer;

    // 清空屏幕缓冲区
    memset(dest, 0, width * height * sizeof(Pixel));

    // 拷贝像素数据
    for (int y = 0; y < copyHeight; y++)
    {
        memcpy(dest + y * width, src + y * pRenderer->m_width, copyWidth * sizeof(Pixel));
    }
}