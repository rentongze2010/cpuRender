#include "Window.h"
#include "Renderer.h"
#include <vector>
#include <cstdint>

Window* g_pWindow = nullptr;
Renderer g_renderer;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 1. 准备初始像素数据
    const int WIDTH = 400;
    const int HEIGHT = 300;
    std::vector<uint32_t> pixelData(WIDTH * HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixelData[i] = 0x000000FF;
    }

    // 2. 初始化渲染器
    g_renderer.SetPixelData(pixelData.data(), WIDTH, HEIGHT);

    // 3. 创建窗口
    Window window;
    g_pWindow = &window;

    if (!window.Create(L"CPU Render Engine v1.0", 800, 600))
    {
        MessageBox(NULL, L"窗口创建失败!", L"错误", MB_ICONERROR);
        return -1;
    }

    // 4. 绑定回调 (在 Show 之前完成!)
    window.SetRenderCallback(Renderer::RenderCallback, &g_renderer);
    window.SetUpdateCallback(Renderer::UpdateCallback, &g_renderer);
    window.SetKeyCallback(Renderer::KeyCallback, &g_renderer);
    window.SetMouseCallback(Renderer::MouseCallback, &g_renderer);

    // 5. 启动定时器
    window.StartTimer(30);

    // 6. 显示窗口 (使用 nCmdShow)
    window.Show(nCmdShow);

    // 7. 触发首次绘制 (回调已就绪)
    UpdateWindow(window.GetHandle());

    // 8. 运行消息循环
    return window.Run();
}