#include "Renderer.h"
#include "Window.h"
#include <cstring>
#include <algorithm>

Renderer::Renderer()
    : m_pixelData(nullptr), m_pixelBuffer(nullptr),
    m_width(0), m_height(0), m_showBall(true),
    m_mouseX(0), m_mouseY(0), m_mousePressed(false)
{
    // 初始化小球
    m_ball.x = 100.0f;
    m_ball.y = 100.0f;
    m_ball.vx = 200.0f; // 像素/秒
    m_ball.vy = 150.0f;
    m_ball.radius = 30.0f;
    m_ball.color = 0xFFFFFFFF; // 白色
}

Renderer::~Renderer()
{
    delete[] m_pixelBuffer;
}

void Renderer::SetPixelData(const uint32_t* pixels, int width, int height)
{
    m_width = width;
    m_height = height;
    delete[] m_pixelBuffer;
    m_pixelBuffer = new Pixel[width * height];
    m_pixelData = (uint32_t*)pixels;

    // 初始化背景
    for (int i = 0; i < width * height; i++)
    {
        uint32_t rgba = pixels[i];
        m_pixelBuffer[i].r = (rgba >> 24) & 0xFF;
        m_pixelBuffer[i].g = (rgba >> 16) & 0xFF;
        m_pixelBuffer[i].b = (rgba >> 8) & 0xFF;
        m_pixelBuffer[i].a = rgba & 0xFF;
    }
}

void Renderer::UpdateCallback(void* userData, float deltaTime)
{
    Renderer* p = (Renderer*)userData;
    if (p) p->UpdateLogic(deltaTime);
}

void Renderer::UpdateLogic(float deltaTime)
{
    if (!m_showBall) return;

    // 移动小球
    m_ball.x += m_ball.vx * deltaTime;
    m_ball.y += m_ball.vy * deltaTime;

    // 边界碰撞检测
    if (m_ball.x - m_ball.radius < 0) { m_ball.x = m_ball.radius; m_ball.vx *= -1; }
    if (m_ball.x + m_ball.radius > m_width) { m_ball.x = m_width - m_ball.radius; m_ball.vx *= -1; }
    if (m_ball.y - m_ball.radius < 0) { m_ball.y = m_ball.radius; m_ball.vy *= -1; }
    if (m_ball.y + m_ball.radius > m_height) { m_ball.y = m_height - m_ball.radius; m_ball.vy *= -1; }

    // 键盘控制 (WASD 加速)
    // 注意：这里需要访问 Window 的按键状态，为了简化，我们在 Main 中传递 Window 指针或通过全局变量
    // 这里演示通过外部全局 Window 指针访问 (实际项目中建议用依赖注入)
    extern Window * g_pWindow;
    if (g_pWindow)
    {
        float speed = 500.0f * deltaTime;
        if (g_pWindow->IsKeyDown('W')) m_ball.y -= speed;
        if (g_pWindow->IsKeyDown('S')) m_ball.y += speed;
        if (g_pWindow->IsKeyDown('A')) m_ball.x -= speed;
        if (g_pWindow->IsKeyDown('D')) m_ball.x += speed;
    }
}

void Renderer::RenderCallback(void* userData, int width, int height, void* pixelBuffer)
{
    Renderer* p = (Renderer*)userData;
    if (!p) return;

    // 1. 清空/绘制背景 (这里简单拷贝初始数据)
    // 实际项目中应该每帧重绘背景
    Pixel* dest = (Pixel*)pixelBuffer;
    memset(dest, 50, width * height * sizeof(Pixel)); // 深灰色背景

    // 2. 绘制小球
    p->DrawBall(pixelBuffer, width, height);

    // 3. 绘制鼠标提示
    if (p->m_mousePressed)
    {
        // 简单在鼠标位置画个红点
        int mx = p->m_mouseX;
        int my = p->m_mouseY;
        if (mx >= 0 && mx < width && my >= 0 && my < height)
        {
            Pixel* pPixel = &dest[my * width + mx];
            pPixel->r = 255; pPixel->g = 0; pPixel->b = 0;
        }
    }
}

void Renderer::DrawBall(void* buffer, int width, int height)
{
    Pixel* pixels = (Pixel*)buffer;
    int cx = (int)m_ball.x;
    int cy = (int)m_ball.y;
    int r = (int)m_ball.radius;

    // 简单的圆形光栅化
    for (int y = -r; y <= r; y++)
    {
        for (int x = -r; x <= r; x++)
        {
            if (x * x + y * y <= r * r)
            {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < width && py >= 0 && py < height)
                {
                    Pixel* p = &pixels[py * width + px];
                    p->r = 255; p->g = 255; p->b = 0; // 黄色球
                    p->a = 255;
                }
            }
        }
    }
}

void Renderer::KeyCallback(void* userData, int key, bool isPressed)
{
    Renderer* p = (Renderer*)userData;
    if (key == 'B' && isPressed) p->m_showBall = !p->m_showBall; // 按 B 隐藏/显示球
}

void Renderer::MouseCallback(void* userData, int x, int y, int button)
{
    Renderer* p = (Renderer*)userData;
    p->m_mouseX = x;
    p->m_mouseY = y;
    p->m_mousePressed = (button == 1);
    if (p->m_mousePressed)
    {
        // 点击小球 teleport
        p->m_ball.x = (float)x;
        p->m_ball.y = (float)y;
    }
}