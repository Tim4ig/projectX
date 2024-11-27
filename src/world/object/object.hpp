#pragma once

#include "resources/d3d11drawable.hpp"

namespace x::world
{
    class Object : public render::Drawable
    {
    public:
        explicit Object(const ComPtr<ID3D11Device>& device) : Drawable(device)
        {
        }

        ~Object() override = default;
    };
}
