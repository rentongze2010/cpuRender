#include "Window.h"
#include "Renderer.h"
#include <vector>
#include <cstdint>

// 全局渲染器实例
Renderer g_renderer;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ========== 1. 准备像素数据 (RGBA 格式) ==========
    const int WIDTH = 400;
    const int HEIGHT = 300;

    std::vector<uint32_t> pixelData(WIDTH * HEIGHT);

    // 生成测试图案 (彩虹渐变)
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // RGBA 格式：0xAABBGGRR (小端序)
            uint8_t r = (uint8_t)(255 * x / WIDTH);
            uint8_t g = (uint8_t)(255 * y / HEIGHT);
            uint8_t b = 128;
            uint8_t a = 255;

            // 打包成 32 位整数 (RGBA)
            pixelData[y * WIDTH + x] = (a << 24) | (b << 16) | (g << 8) | r;
        }
    }

    // ========== 2. 设置渲染器 ==========
    g_renderer.SetPixelData(pixelData.data(), WIDTH, HEIGHT);

    // ========== 3. 创建窗口 ==========
    Window window;
    if (!window.Create(L"C++ Win32 RGBA 渲染示例", 800, 600))
    {
        MessageBox(NULL, L"窗口创建失败!", L"错误", MB_ICONERROR);
        return -1;
    }

    // 设置渲染回调
    window.SetRenderCallback(Renderer::RenderCallback, &g_renderer);

    // ========== 4. 显示并运行 ==========
    window.Show();
    return window.Run();
}