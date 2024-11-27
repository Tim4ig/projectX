#pragma once

#include "drawable_wrapper.hpp"
#include "transform.hpp"

namespace x::world
{
    class Object : public DrawableWrapper, public Transform
    {
    public:
        explicit Object(const ComPtr<ID3D11Device>& device) : DrawableWrapper(device)
        {
            Update();
        }

        void Update() override
        {
            Transform::Update();
            auto const matrix = GetWorldMatrix();
            m_SetData(&matrix, sizeof(matrix));
        }

        ~Object() override = default;
    };
}
