#pragma once
#include <windows.h>

class Input {
public:
    Input();
    ~Input();

    // 更新输入状态
    void update();

    // 3. 键盘状态
    bool isKeyDown(int vkCode) const;
    bool isKeyPressed(int vkCode) const;
    bool isKeyReleased(int vkCode) const;

    // 鼠标状态
    bool isMouseButtonDown(int button) const; // 0=左, 1=右, 2=中
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonReleased(int button) const;

    // 获取鼠标位置
    void getMousePosition(int& x, int& y) const;

    // 获取相对于窗口的鼠标位置
    void getMousePositionInWindow(int& x, int& y, HWND hWnd) const;

private:
    bool keyStates[256];      // 当前按键状态
    bool prevKeyStates[256];  // 上一帧按键状态
    bool mouseButtons[3];     // 鼠标按钮状态
    bool prevMouseButtons[3]; // 上一帧鼠标按钮状态
    POINT mousePos;           // 屏幕坐标系中的鼠标位置
    POINT mousePosInWindow;   // 窗口坐标系中的鼠标位置
};