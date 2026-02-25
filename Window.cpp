#include "Window.h"
#include <cstring>

void clearScreen(std::vector<Pixel>& screen, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Pixel clearColor = { b, g, r, a };
    std::fill(screen.begin(), screen.end(), clearColor);
}

Window::Window() :
    hWnd(nullptr),
    hdcMem(nullptr),
    hBitmap(nullptr),
    hOldBitmap(nullptr),
    screenBits(nullptr),
    screenWidth(0),
    screenHeight(0),
    fullscreenMode(false)
{
}

Window::~Window() {
    close();
}

bool Window::create(int width, int height, const wchar_t* title) {
    cleanup(); // 确保清理之前的状态

    screenWidth = width;
    screenHeight = height;

    // 注册窗口类
    const wchar_t CLASS_NAME[] = L"GameWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = DefWindowProcW;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    // 创建窗口
    DWORD style = fullscreenMode ? WS_POPUP : WS_OVERLAPPEDWINDOW;

    hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        GetModuleHandleW(NULL),
        NULL
    );

    if (hWnd == NULL) {
        return false;
    }

    if (fullscreenMode) {
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
    }
    else {
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);
    }

    return initializeBuffer();
}

void Window::close() {
    cleanup();

    if (hWnd) {
        DestroyWindow(hWnd);
        hWnd = nullptr;
    }
}

bool Window::initializeBuffer() {
    if (!hWnd) return false;

    cleanup(); // 确保清理之前的缓冲区

    // 创建内存DC用于双缓冲
    hdcMem = CreateCompatibleDC(GetDC(hWnd));
    if (!hdcMem) {
        return false;
    }

    // 创建与屏幕兼容的位图
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = screenWidth;
    bmi.bmiHeader.biHeight = -screenHeight;  // 负值表示顶向下DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits;
    hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &bits, NULL, 0x0);
    if (!hBitmap) {
        cleanup();
        return false;
    }

    screenBits = static_cast<uint8_t*>(bits);
    hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    return true;
}

void Window::cleanup() {
    if (hdcMem) {
        if (hOldBitmap) {
            SelectObject(hdcMem, hOldBitmap);
            hOldBitmap = nullptr;
        }

        if (hBitmap) {
            DeleteObject(hBitmap);
            hBitmap = nullptr;
        }

        DeleteDC(hdcMem);
        hdcMem = nullptr;
    }

    screenBits = nullptr;
}

// 1. 刷新屏幕
void Window::update(std::vector<Pixel>& screen) {
    if (!hdcMem || !hBitmap || !hWnd || !screenBits) {
        return;
    }

    // 将像素数据直接复制到DIBSection
    size_t totalBytes = screenWidth * screenHeight * sizeof(Pixel);

    // 确保screen有足够的数据
    if (screen.size() * sizeof(Pixel) >= totalBytes) {
        memcpy(screenBits, screen.data(), totalBytes);

        // 将内存DC的内容复制到窗口DC
        HDC hdc = GetDC(hWnd);
        if (hdc) {
            BitBlt(hdc, 0, 0, screenWidth, screenHeight, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(hWnd, hdc);
        }
    }
}

// 2. 全屏与分辨率设置
bool Window::setFullscreen(bool fullscreen) {
    if (fullscreen == fullscreenMode) {
        return true; // 已经处于所需状态
    }

    int currentWidth = screenWidth;
    int currentHeight = screenHeight;

    if (fullscreen) {
        // 获取当前屏幕分辨率
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // 设置全屏显示模式
        DEVMODEW devMode = {};
        devMode.dmSize = sizeof(DEVMODEW);
        devMode.dmPelsWidth = width;
        devMode.dmPelsHeight = height;
        devMode.dmBitsPerPel = 32;
        devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        LONG result = ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN);
        if (result != DISP_CHANGE_SUCCESSFUL) {
            return false;
        }

        // 创建全屏窗口
        SetWindowLongW(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);

        screenWidth = width;
        screenHeight = height;
    }
    else {
        // 恢复窗口模式
        ChangeDisplaySettingsW(NULL, 0);

        // 恢复窗口样式和位置
        SetWindowLongW(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(hWnd, HWND_TOP, 100, 100, currentWidth, currentHeight, SWP_SHOWWINDOW);

        screenWidth = currentWidth;
        screenHeight = currentHeight;
    }

    fullscreenMode = fullscreen;

    // 重新初始化位图以适应新分辨率
    return initializeBuffer();
}

bool Window::setResolution(int width, int height) {
    if (width <= 0 || height <= 0) {
        return false;
    }

    screenWidth = width;
    screenHeight = height;

    if (fullscreenMode) {
        // 在全屏模式下更改分辨率
        DEVMODEW devMode = {};
        devMode.dmSize = sizeof(DEVMODEW);
        devMode.dmPelsWidth = width;
        devMode.dmPelsHeight = height;
        devMode.dmBitsPerPel = 32;
        devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        LONG result = ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN);
        if (result != DISP_CHANGE_SUCCESSFUL) {
            return false;
        }

        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
    }
    else {
        // 在窗口模式下更改分辨率
        SetWindowPos(hWnd, HWND_TOP, 100, 100, width, height, SWP_SHOWWINDOW);
    }

    return initializeBuffer();
}

void Window::getResolution(int& width, int& height) const {
    width = screenWidth;
    height = screenHeight;
}

bool Window::isFullscreen() const {
    return fullscreenMode;
}

bool Window::processMessages() {
    MSG msg = {};
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}

HWND Window::getHandle() const {
    return hWnd;
}

size_t Window::getBufferSize() const {
    return screenWidth * screenHeight;
}