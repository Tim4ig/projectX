#pragma once

#include "render/resources/camera.hpp"

#include "logger.hpp"

namespace x::develop
{
    class CameraController
    {
    public:
        CameraController(render::Camera* camera) : m_camera(camera)
        {
        }

        void Update()
        {
            auto tranlation = DirectX::XMVectorSet(0, 0, 0, 0);

            static auto isRightButtonPressed = false;

            DirectX::XMFLOAT3 offset;

            static auto a = std::numbers::pi_v<float> / 2;
            static auto b = std::numbers::pi_v<float> / 2;

            static POINT lastCursorPos = {600, 600};
            if (GetAsyncKeyState(VK_RBUTTON))
            {
                POINT cursorPos;

                if (!isRightButtonPressed) SetCursorPos(600, 600);

                GetCursorPos(&cursorPos);
                SetCursorPos(600, 600);

                float offsetX = lastCursorPos.x - cursorPos.x;
                float offsetY = lastCursorPos.y - cursorPos.y;

                offsetX *= 0.001f;
                offsetY *= -0.001f;

                a += offsetX;
                b += offsetY;
            }
            else
                isRightButtonPressed = false;

            auto x = cos(a) * sin(b);
            auto y = cos(b);
            auto z = sin(a) * sin(b);

            DirectX::XMStoreFloat3(&offset, DirectX::XMVectorSet(x, y, z, 0));

            m_camera->Rotate(offset);

            isRightButtonPressed = true;

            if (GetAsyncKeyState('W'))
                tranlation = DirectX::XMVectorAdd(tranlation, DirectX::XMLoadFloat3(&offset));
            if (GetAsyncKeyState('S'))
                tranlation = DirectX::XMVectorSubtract(tranlation, DirectX::XMLoadFloat3(&offset));

            if (GetAsyncKeyState('A'))
                tranlation = DirectX::XMVectorAdd(tranlation, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&offset), DirectX::XMVectorSet(0, 1, 0, 0)));
            if (GetAsyncKeyState('D'))
                tranlation = DirectX::XMVectorAdd(tranlation, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&offset), DirectX::XMVectorSet(0, -1, 0, 0)));

            if (GetAsyncKeyState(VK_SPACE))
                tranlation = DirectX::XMVectorAdd(tranlation, DirectX::XMVectorSet(0, 1, 0, 0));
            if (GetAsyncKeyState(VK_SHIFT))
                tranlation = DirectX::XMVectorAdd(tranlation, DirectX::XMVectorSet(0, -1, 0, 0));

            static float speed = 1;

            if (GetAsyncKeyState('T')) speed += 0.1f;
            if (GetAsyncKeyState('Y')) speed -= 0.1f;

            DirectX::XMFLOAT3 translation;
            tranlation = DirectX::XMVectorMultiply(tranlation, DirectX::XMVectorSet(0.1f * speed, 0.1f * speed, 0.1f * speed, 0));
            DirectX::XMStoreFloat3(&translation, tranlation);

            m_camera->Move(translation);
        }

    private:
        render::Camera* m_camera;
    };
}
