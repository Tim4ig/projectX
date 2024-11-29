#pragma once

#include "pch.h"

namespace x::world
{
    class Transform
    {
    public:
        Transform() :
            m_scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
            m_position(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
            m_rotation(DirectX::XMMatrixIdentity()),
            m_worldMatrix(DirectX::XMMatrixIdentity())
        {
        }

        ~Transform() = default;

        void SetScale(const DirectX::XMFLOAT3& scale)
        {
            m_scale = scale;
        }

        void SetPosition(const DirectX::XMFLOAT3& position)
        {
            m_position = position;
        }

        void SetRotation(const DirectX::XMMATRIX& rotation)
        {
            m_rotation = rotation;
        }

        void SetRotation(const DirectX::XMFLOAT4& rotation)
        {
            m_rotation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
        }

        void Translate(const DirectX::XMFLOAT3& translation)
        {
            m_position.x += translation.x;
            m_position.y += translation.y;
            m_position.z += translation.z;
        }

        [[nodiscard]] const DirectX::XMFLOAT3& GetScale() const
        {
            return m_scale;
        }

        [[nodiscard]] const DirectX::XMFLOAT3& GetPosition() const
        {
            return m_position;
        }

        [[nodiscard]] const DirectX::XMMATRIX& GetRotation() const
        {
            return m_rotation;
        }

        [[nodiscard]] const DirectX::XMMATRIX& GetWorldMatrix() const
        {
            return m_worldMatrix;
        }

    private:
        DirectX::XMFLOAT3 m_scale;
        DirectX::XMFLOAT3 m_position;
        DirectX::XMMATRIX m_rotation;
        DirectX::XMMATRIX m_worldMatrix;

    protected:
        virtual void Update()
        {
            m_worldMatrix =
                m_rotation;
        }
    };
}
