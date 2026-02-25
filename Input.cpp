#include "Input.h"
#include <cstring>

Input::Input() {
    memset(keyStates, 0, sizeof(keyStates));
    memset(prevKeyStates, 0, sizeof(prevKeyStates));
    memset(mouseButtons, 0, sizeof(mouseButtons));
    memset(prevMouseButtons, 0, sizeof(prevMouseButtons));

    GetCursorPos(&mousePos);
    mousePosInWindow = mousePos;
}

Input::~Input() {
    // 无需特殊清理
}

void Input::update() {
    // 保存上一帧状态
    memcpy(prevKeyStates, keyStates, sizeof(keyStates));
    memcpy(prevMouseButtons, mouseButtons, sizeof(mouseButtons));

    // 使用GetAsyncKeyState更新当前按键状态
    for (int i = 0; i < 256; i++) {
        keyStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    // 更新鼠标位置
    GetCursorPos(&mousePos);

    // 更新鼠标按钮状态
    mouseButtons[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    mouseButtons[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    mouseButtons[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

// 键盘状态检测
bool Input::isKeyDown(int vkCode) const {
    return keyStates[vkCode];
}

bool Input::isKeyPressed(int vkCode) const {
    return keyStates[vkCode] && !prevKeyStates[vkCode];
}

bool Input::isKeyReleased(int vkCode) const {
    return !keyStates[vkCode] && prevKeyStates[vkCode];
}

// 鼠标状态检测
bool Input::isMouseButtonDown(int button) const {
    return (button >= 0 && button < 3) ? mouseButtons[button] : false;
}

bool Input::isMouseButtonPressed(int button) const {
    return (button >= 0 && button < 3) ? (mouseButtons[button] && !prevMouseButtons[button]) : false;
}

bool Input::isMouseButtonReleased(int button) const {
    return (button >= 0 && button < 3) ? (!mouseButtons[button] && prevMouseButtons[button]) : false;
}

void Input::getMousePosition(int& x, int& y) const {
    x = mousePos.x;
    y = mousePos.y;
}

void Input::getMousePositionInWindow(int& x, int& y, HWND hWnd) const {
    POINT pos = mousePos;
    ScreenToClient(hWnd, &pos);
    x = pos.x;
    y = pos.y;
}