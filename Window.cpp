#include "Window.h"

Window::Window()
    : m_hWnd(NULL), m_width(0), m_height(0),
    m_renderCallback(nullptr), m_renderUserData(nullptr),
    m_initialized(false)
{
}

Window::~Window()
{
    if (m_hWnd)
    {
        DestroyWindow(m_hWnd);
    }
}

bool Window::Create(const wchar_t* title, int width, int height)
{
    m_width = width;
    m_height = height;

    // 注册窗口类
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProcStatic;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"PixelWindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    // 创建窗口
    m_hWnd = CreateWindowEx(
        0,
        L"PixelWindowClass",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL,
        GetModuleHandle(NULL),
        this  // 将 this 指针传递给窗口
    );

    if (!m_hWnd)
    {
        return false;
    }

    m_initialized = true;
    return true;
}

void Window::Show()
{
    if (m_hWnd)
    {
        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);
    }
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

void Window::SetRenderCallback(RenderCallback callback, void* userData)
{
    m_renderCallback = callback;
    m_renderUserData = userData;
}

void Window::Invalidate()
{
    if (m_hWnd)
    {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // 获取 Window 实例指针
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

    if (pWindow)
    {
        return pWindow->WndProc(hwnd, msg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdcScreen = BeginPaint(hwnd, &ps);

        // 获取客户区尺寸
        RECT rc;
        GetClientRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        if (width > 0 && height > 0 && m_renderCallback)
        {
            // 创建内存 DC 和位图
            HDC hdcMem = CreateCompatibleDC(hdcScreen);

            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;  // 负值表示自上而下
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            void* pixels = nullptr;
            HBITMAP hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);

            if (hBitmap && pixels)
            {
                HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

                // 调用用户渲染回调
                m_renderCallback(m_renderUserData, width, height, pixels);

                // 拷贝到屏幕
                BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, hOld);
                DeleteObject(hBitmap);
            }

            DeleteDC(hdcMem);
        }

        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}