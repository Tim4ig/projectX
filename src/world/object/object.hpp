#pragma once

#include "drawable_wrapper.hpp"
#include "transform.hpp"

namespace x::world
{
    class Object : public DrawableWrapper, public Transform
    {
    public:
        explicit Object(std::unique_ptr<render::Drawable>& drawable) : DrawableWrapper(drawable)
        {
            Update();
        }

        void Update() override
        {
            Transform::Update();
            auto const matrix = GetWorldMatrix();
            m_SetConstantBuffer(&matrix, sizeof(matrix));
        }

        ~Object() override = default;
    };
}
