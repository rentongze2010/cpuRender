#pragma once
#include <windows.h>
#include <functional>

class Window
{
public:
    Window();
    ~Window();

    // 创建窗口
    bool Create(const wchar_t* title, int width, int height);

    // 消息循环
    int Run();

    void Show(int nCmdShow = SW_SHOW);

    // --- 动画与渲染 ---
    using RenderCallback = void (*)(void* userData, int width, int height, void* pixelBuffer);
    using UpdateCallback = void (*)(void* userData, float deltaTime);

    void SetRenderCallback(RenderCallback callback, void* userData);
    void SetUpdateCallback(UpdateCallback callback, void* userData);
    void StartTimer(int intervalMs = 16); // 默认约 60FPS
    void StopTimer();

    // --- 输入系统 ---
    using KeyCallback = void (*)(void* userData, int key, bool isPressed);
    using MouseCallback = void (*)(void* userData, int x, int y, int button);

    void SetKeyCallback(KeyCallback callback, void* userData);
    void SetMouseCallback(MouseCallback callback, void* userData);

    // 查询按键状态 (用于连续移动)
    bool IsKeyDown(int keyCode) const;

    // --- 全屏控制 ---
    void ToggleFullscreen();
    bool IsFullscreen() const { return m_isFullscreen; }

    // 获取尺寸
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    HWND GetHandle() const { return m_hWnd; }

private:
    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void UpdateWindowSize();

    HWND m_hWnd;
    int m_width;
    int m_height;
    bool m_isFullscreen;

    // 保存全屏前的窗口状态
    RECT m_windowedRect;
    LONG_PTR m_windowedStyle;
    LONG_PTR m_windowedExStyle;

    // 回调与数据
    RenderCallback m_renderCallback;
    void* m_renderUserData;
    UpdateCallback m_updateCallback;
    void* m_updateUserData;
    KeyCallback m_keyCallback;
    void* m_keyUserData;
    MouseCallback m_mouseCallback;
    void* m_mouseUserData;

    // 按键状态表
    bool m_keys[256];
};