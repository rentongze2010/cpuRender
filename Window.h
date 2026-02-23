#pragma once
#include <windows.h>
#include <string>

class Window
{
public:
    Window();
    ~Window();

    // 创建窗口
    bool Create(const wchar_t* title, int width, int height);

    // 显示窗口
    void Show();

    // 消息循环
    int Run();

    // 获取窗口句柄
    HWND GetHandle() const { return m_hWnd; }

    // 获取客户区尺寸
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // 设置渲染回调
    using RenderCallback = void (*)(void* userData, int width, int height, void* pixelBuffer);
    void SetRenderCallback(RenderCallback callback, void* userData);

    // 请求重绘
    void Invalidate();

private:
    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND m_hWnd;
    int m_width;
    int m_height;
    RenderCallback m_renderCallback;
    void* m_renderUserData;
    bool m_initialized;
};