#include "Window.h"

Window::Window()
    : m_hWnd(NULL), m_width(0), m_height(0), m_isFullscreen(false),
    m_renderCallback(nullptr), m_updateCallback(nullptr),
    m_keyCallback(nullptr), m_mouseCallback(nullptr),
    m_renderUserData(nullptr), m_updateUserData(nullptr),
    m_keyUserData(nullptr), m_mouseUserData(nullptr)
{
    // 初始化按键状态
    for (int i = 0; i < 256; i++) m_keys[i] = false;
}

Window::~Window()
{
    StopTimer();
    if (m_hWnd) DestroyWindow(m_hWnd);
}

bool Window::Create(const wchar_t* title, int width, int height)
{
    m_width = width;
    m_height = height;

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProcStatic;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // 默认黑色背景
    wc.lpszClassName = L"CPURenderWindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) return false;

    // 计算窗口大小，使客户区正好为 width x height
    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    int winWidth = rect.right - rect.left;
    int winHeight = rect.bottom - rect.top;

    m_hWnd = CreateWindowEx(
        0, L"CPURenderWindowClass", title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winWidth, winHeight,
        NULL, NULL, GetModuleHandle(NULL), this
    );

    if (!m_hWnd) return false;

    // 保存初始状态用于恢复全屏
    m_windowedStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
    m_windowedExStyle = GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
    GetWindowRect(m_hWnd, &m_windowedRect);

    return true;
}

int Window::Run()
{
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

void Window::Show(int nCmdShow)
{
    if (m_hWnd)
    {
        ShowWindow(m_hWnd, nCmdShow);
        // 不在这里调用 UpdateWindow，交给 main 控制时机
    }
}

void Window::SetRenderCallback(RenderCallback callback, void* userData) {
    m_renderCallback = callback; m_renderUserData = userData;
}
void Window::SetUpdateCallback(UpdateCallback callback, void* userData) {
    m_updateCallback = callback; m_updateUserData = userData;
}
void Window::SetKeyCallback(KeyCallback callback, void* userData) {
    m_keyCallback = callback; m_keyUserData = userData;
}
void Window::SetMouseCallback(MouseCallback callback, void* userData) {
    m_mouseCallback = callback; m_mouseUserData = userData;
}

void Window::StartTimer(int intervalMs)
{
    if (m_hWnd) SetTimer(m_hWnd, 1, intervalMs, NULL);
}

void Window::StopTimer()
{
    if (m_hWnd) KillTimer(m_hWnd, 1);
}

bool Window::IsKeyDown(int keyCode) const
{
    return (keyCode >= 0 && keyCode < 256) ? m_keys[keyCode] : false;
}

void Window::ToggleFullscreen()
{
    if (!m_hWnd) return;

    m_isFullscreen = !m_isFullscreen;

    if (m_isFullscreen)
    {
        // 保存当前状态
        GetWindowRect(m_hWnd, &m_windowedRect);
        m_windowedStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
        m_windowedExStyle = GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);

        // 设置为无边框弹出窗口
        SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
        SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

        // 获取显示器工作区 (不覆盖任务栏)
        HMONITOR hMonitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo = { sizeof(monitorInfo) };
        GetMonitorInfo(hMonitor, &monitorInfo);

        SetWindowPos(m_hWnd, HWND_TOPMOST,
            monitorInfo.rcWork.left, monitorInfo.rcWork.top,
            monitorInfo.rcWork.right - monitorInfo.rcWork.left,
            monitorInfo.rcWork.bottom - monitorInfo.rcWork.top,
            SWP_FRAMECHANGED);
    }
    else
    {
        // 恢复窗口状态
        SetWindowLongPtr(m_hWnd, GWL_STYLE, m_windowedStyle);
        SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, m_windowedExStyle);

        SetWindowPos(m_hWnd, HWND_NOTOPMOST,
            m_windowedRect.left, m_windowedRect.top,
            m_windowedRect.right - m_windowedRect.left,
            m_windowedRect.bottom - m_windowedRect.top,
            SWP_FRAMECHANGED);
    }
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindowSize(); // 更新内部记录的尺寸
}

void Window::UpdateWindowSize()
{
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = nullptr;
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pWindow = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
        pWindow->m_hWnd = hwnd;
    }
    else
    {
        pWindow = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pWindow) return pWindow->WndProc(hwnd, msg, wParam, lParam);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TIMER:
        if (wParam == 1 && m_updateCallback)
        {
            // 调用 Update 逻辑 (deltaTime 设为固定值 0.016s)
            m_updateCallback(m_updateUserData, 0.016f);
            // 更新后请求重绘
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdcScreen = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        m_width = width; m_height = height; // 同步尺寸

        if (width > 0 && height > 0 && m_renderCallback)
        {
            HDC hdcMem = CreateCompatibleDC(hdcScreen);
            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            void* pixels = nullptr;
            HBITMAP hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);

            if (hBitmap && pixels)
            {
                HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
                // 执行渲染
                m_renderCallback(m_renderUserData, width, height, pixels);
                BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hdcMem, hOld);
                DeleteObject(hBitmap);
            }
            DeleteDC(hdcMem);
        }
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_KEYDOWN:
    {
        int key = (int)wParam;
        m_keys[key] = true;
        // 全屏快捷键 F11 或 F
        if (key == VK_F11 || key == 'F') ToggleFullscreen();
        if (m_keyCallback) m_keyCallback(m_keyUserData, key, true);
    }
    return 0;

    case WM_KEYUP:
    {
        int key = (int)wParam;
        m_keys[key] = false;
        if (m_keyCallback) m_keyCallback(m_keyUserData, key, false);
    }
    return 0;

    case WM_MOUSEMOVE:
        if (m_mouseCallback)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            m_mouseCallback(m_mouseUserData, x, y, 0);
        }
        return 0;

    case WM_LBUTTONDOWN:
        if (m_mouseCallback)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            m_mouseCallback(m_mouseUserData, x, y, 1); // 1 代表左键按下
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}