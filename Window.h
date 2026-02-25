#pragma once
#include <vector>
#include <windows.h>

struct Pixel
{
    uint8_t b;  // Blue
    uint8_t g;  // Green
    uint8_t r;  // Red
    uint8_t a;  // Alpha
};

// 清空屏幕函数
void clearScreen(std::vector<Pixel>& screen, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

class Window {
public:
    Window();
    ~Window();

    bool create(int width, int height, const wchar_t* title = L"Desktop Application");
    void close();

    // 1. 刷新屏幕
    void update(std::vector<Pixel>& screen);

    // 2. 全屏与分辨率设置
    bool setFullscreen(bool fullscreen);
    bool setResolution(int width, int height);
    void getResolution(int& width, int& height) const;
    bool isFullscreen() const;

    // 消息处理
    bool processMessages();
    HWND getHandle() const;

    // 获取屏幕缓冲区大小
    size_t getBufferSize() const;

private:
    HWND hWnd;
    HDC hdcMem;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;
    uint8_t* screenBits;
    int screenWidth;
    int screenHeight;
    bool fullscreenMode;

    bool initializeBuffer();
    void cleanup();
};