#pragma once

namespace x::render::drawable
{
    class Transform
    {
    public:
        virtual ~Transform() = default;

        void SetPosition(const DirectX::XMVECTOR pos) { m_position = pos; }
        void SetRotation(const DirectX::XMVECTOR rot) { m_rotation = rot; }
        void SetScale(const DirectX::XMVECTOR scale) { m_scale = scale; }

        void SetPosition(const float x, const float y, const float z) { m_position = DirectX::XMVectorSet(x, y, z, 1.0f); }
        void SetRotation(const float x, const float y, const float z) { m_rotation = DirectX::XMVectorSet(x, y, z, 1.0f); }
        void SetScale(const float x, const float y, const float z) { m_scale = DirectX::XMVectorSet(x, y, z, 1.0f); }

        void Move(const DirectX::XMVECTOR pos) { m_position = DirectX::XMVectorAdd(m_position, pos); }
        void Rotate(const DirectX::XMVECTOR rot) { m_rotation = DirectX::XMVectorAdd(m_rotation, rot); }
        void Scale(const DirectX::XMVECTOR scale) { m_scale = DirectX::XMVectorAdd(m_scale, scale); }

        void Move(const float x, const float y, const float z) { m_position = DirectX::XMVectorAdd(m_position, DirectX::XMVectorSet(x, y, z, 1.0f)); }
        void Rotate(const float x, const float y, const float z) { m_rotation = DirectX::XMVectorAdd(m_rotation, DirectX::XMVectorSet(x, y, z, 1.0f)); }
        void Scale(const float x, const float y, const float z) { m_scale = DirectX::XMVectorAdd(m_scale, DirectX::XMVectorSet(x, y, z, 1.0f)); }

        DirectX::XMVECTOR GetPosition() const { return m_position; }
        DirectX::XMVECTOR GetRotation() const { return m_rotation; }
        DirectX::XMVECTOR GetScale() const { return m_scale; }

        DirectX::XMMATRIX GetWorldMatrix() const { return m_worldMatrix; }

        virtual void Update()
        {
            Update(DirectX::XMMatrixIdentity());
        }

        void Update(const DirectX::XMMATRIX& parent)
        {
            m_worldMatrix =
                DirectX::XMMatrixScalingFromVector(m_scale) *
                DirectX::XMMatrixRotationRollPitchYawFromVector(m_rotation) *
                DirectX::XMMatrixTranslationFromVector(m_position);

            m_worldMatrix = XMMatrixMultiply(m_worldMatrix, parent);
        }

    private:
        DirectX::XMVECTOR m_position = DirectX::XMVectorSet(1, 1, 1, 1);
        DirectX::XMVECTOR m_rotation = DirectX::g_XMZero;
        DirectX::XMVECTOR m_scale = DirectX::XMVectorSet(1, 1, 1, 1);

        DirectX::XMMATRIX m_worldMatrix = DirectX::XMMatrixIdentity();
    };
}
