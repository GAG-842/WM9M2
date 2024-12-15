#pragma once
#include "window.h"
#include "mathcal.h"
#include "shader.h"
#include "Animation.h"
class camera {
public:
    Vec3 position = Vec3(0.f, 1.5f, 0.f); // 摄像头位置
    Vec3 forward = Vec3(0.f, 0.f, 1.f);  // 摄像头正前方向
    Vec3 up = Vec3(0.f, 1.f, 0.f);       // 摄像头上方向
    Vec3 right = Vec3(1.f, 0.f, 0.f);    // 摄像头右方向
    float pitch = 0.f;                   // 俯仰角
    float yaw = 90.f;                    // 偏航角（默认朝 Z 轴正方向）
    float speed = 0.1f;                  // 移动速度
    float sensitivity = 0.1f;            // 鼠标灵敏度

    HWND hwnd;                           // 窗口句柄
    POINT center;                        // 窗口中心点

    // 初始化摄像头
    void init(HWND windowHandle) {
        hwnd = windowHandle;
        RECT rect;
        GetClientRect(hwnd, &rect);
        center.x = (rect.left + rect.right) / 2;
        center.y = (rect.top + rect.bottom) / 2;

        // 隐藏鼠标指针
        ShowCursor(FALSE);
        // 限制鼠标到窗口内
        ClipCursor(&rect);
    }

    // 更新摄像头
    void update(Window* canvas, shader* sd) {
        handleMouseInput(); // 处理鼠标输入
        handleMovement(canvas); // 处理键盘输入
        updateCameraMatrix(sd); // 更新摄像头矩阵
    }


private:
    void handleMouseInput() {
        POINT mousePos;
        GetCursorPos(&mousePos);

        // 计算鼠标相对于窗口中心的偏移量
        float deltaX = (mousePos.x - center.x) * sensitivity;
        float deltaY = (mousePos.y - center.y) * sensitivity;

        // 更新摄像头的偏航角和俯仰角
        yaw -= deltaX;
        pitch -= deltaY;

        // 限制俯仰角范围
        if (pitch > 45.f) pitch = 45.f;
        if (pitch < -45.f) pitch = -45.f;

        // 更新方向向量
        updateDirections();

        // 重置鼠标位置到窗口中心
        SetCursorPos(center.x, center.y);
    }

    // 键盘输入处理
    void handleMovement(Window* canvas) {
        if (canvas->keyPressed('W')) 
        {
            position.x += forward.x * speed;
            position.z += forward.z * speed;
        }// 向前
        if (canvas->keyPressed('S')) 
        {
            position.x -= forward.x * speed;
            position.z -= forward.z * speed;
        }  // 向后
        if (canvas->keyPressed('A')) position -= right * speed;    // 向左
        if (canvas->keyPressed('D')) position += right * speed;    // 向右
        if (canvas->keyPressed(VK_SPACE)) position.y += speed;     // 向上
        if (canvas->keyPressed(VK_CONTROL)) position.y -= speed;   // 向下
    }

    // 更新方向向量
    void updateDirections() {
        forward = Vec3(
            cosf(yaw * 0.0174533f) * cosf(pitch * 0.0174533f),
            sinf(pitch * 0.0174533f),
            sinf(yaw * 0.0174533f) * cosf(pitch * 0.0174533f)
        ).normalize();

        right = forward.Cross(up).normalize();
    }

    // 更新摄像头矩阵
    void updateCameraMatrix(shader* sd) {
        Vec3 target = position + forward;
        Matrix viewMatrix = Matrix().lookAt(position, target, up);
        Matrix projectionMatrix = Matrix().perspective(0.9f, 1024.f / 768.f, 0.1f, 100000.f);

        Matrix vp = viewMatrix * projectionMatrix;
        sd->updateConstantVS("MeshBuffer", "VP", &vp);
    }
};