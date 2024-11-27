#pragma once

#include "pch.h"

namespace x::render
{
    class Camera
    {
    public:
        Camera() : m_position({ 0.0f, 0.0f, -5.0f }), m_target({ 0.0f, 0.0f, 1.0f }), m_scale({ 1.0f, 1.0f, 1.0f })
        {
            m_viewMatrix = DirectX::XMMatrixIdentity();
            m_projectionMatrix = DirectX::XMMatrixIdentity();
            m_viewProjectionMatrix = DirectX::XMMatrixIdentity();
        }

        void SetPosition(const DirectX::XMFLOAT3& position) { m_position = position; }
        void SetRotation(const DirectX::XMFLOAT3& rotation) { m_target = rotation; }
        void SetScale(const DirectX::XMFLOAT3& scale) { m_scale = scale; }

        [[nodiscard]] DirectX::XMFLOAT3 GetPosition() const { return m_position; }
        [[nodiscard]] DirectX::XMFLOAT3 GetRotation() const { return m_target; }
        [[nodiscard]] DirectX::XMFLOAT3 GetScale() const { return m_scale; }

        void Move(const DirectX::XMFLOAT3& direction) { m_position.x += direction.x; m_position.y += direction.y; m_position.z += direction.z; }
        void Rotate(const DirectX::XMFLOAT3& rotation) { m_target.x += rotation.x; m_target.y += rotation.y; m_target.z += rotation.z; }
        void Scale(const DirectX::XMFLOAT3& scale) { m_scale.x += scale.x; m_scale.y += scale.y; m_scale.z += scale.z; }

        void UpdateViewMatrix()
        {
            XMStoreFloat3(&m_target, DirectX::XMVector4Normalize(XMLoadFloat3(&m_target)));
            const auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
            const auto focus = DirectX::XMVectorAdd(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target));

            m_viewMatrix = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&m_position), focus, up);
        }

        void UpdateProjectionMatrix(const float aspectRatio, const float fov)
        {
            m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 1000.0f);
        }

        void UpdateViewProjectionMatrix()
        {
            UpdateViewMatrix();
            m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
        }

        [[nodiscard]] DirectX::XMMATRIX GetViewMatrix() const { return m_viewMatrix; }
        [[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() const { return m_projectionMatrix; }
        [[nodiscard]] DirectX::XMMATRIX GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

    private:
        DirectX::XMFLOAT3 m_position;
        DirectX::XMFLOAT3 m_target;
        DirectX::XMFLOAT3 m_scale;

        DirectX::XMMATRIX m_viewMatrix;
        DirectX::XMMATRIX m_projectionMatrix;
        DirectX::XMMATRIX m_viewProjectionMatrix;
    };
}
