#include "Window.h"
#include "Input.h"
#include <iostream>

// Windows API 入口点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 创建窗口
    Window window;
    if (!window.create(800, 600, L"CPU Renderer")) {
        MessageBoxW(NULL, L"Failed to create window", L"Error", MB_OK);
        return 1;
    }

    // 创建输入管理器
    Input input;

    // 初始化屏幕缓冲区
    int width, height;
    window.getResolution(width, height);
    std::vector<Pixel> screen(width * height);

    // 清空屏幕为黑色
    clearScreen(screen);

    bool running = true;
    while (running) {
        // 处理窗口消息
        if (!window.processMessages()) {
            break;
        }

        // 更新输入
        input.update();

        // 检查退出条件
        if (input.isKeyPressed(VK_ESCAPE)) {
            running = false;
        }

        // 切换全屏模式
        if (input.isKeyPressed(VK_F11)) {
            window.setFullscreen(!window.isFullscreen());
            window.getResolution(width, height);
            screen.resize(width * height);
            clearScreen(screen);
        }

        // 清空屏幕 (按C键)
        if (input.isKeyPressed('C')) {
            clearScreen(screen, 0, 0, 255); // 蓝色
        }

        // 绘制一个移动的方块
        static int x = 0, y = 0;
        static int dx = 2, dy = 2;

        // 清除上一帧的方块
        for (int j = y; j < y + 20; j++) {
            for (int i = x; i < x + 20; i++) {
                if (i >= 0 && i < width && j >= 0 && j < height) {
                    size_t index = j * width + i;
                    screen[index] = { 0, 0, 0, 255 }; // 黑色
                }
            }
        }

        // 更新位置
        x += dx;
        y += dy;

        if (x <= 0 || x >= width - 20) dx = -dx;
        if (y <= 0 || y >= height - 20) dy = -dy;

        // 绘制红色方块
        for (int j = y; j < y + 20; j++) {
            for (int i = x; i < x + 20; i++) {
                if (i >= 0 && i < width && j >= 0 && j < height) {
                    size_t index = j * width + i;
                    screen[index] = { 0, 0, 255, 255 }; // 红色
                }
            }
        }

        // 刷新屏幕
        window.update(screen);

        // 简单延迟以控制帧率 (~60 FPS)
        Sleep(16);
    }

    return 0;
}