#pragma once

#include "render/resources/drawable/transform.hpp"
#include "render/resources/drawable/drawable.hpp"

namespace x::world
{
    class Object : public render::drawable::Transform
    {
    public:
        explicit Object(std::unique_ptr<render::drawable::Drawable>& drawable) : m_drawable(std::move(drawable))
        {
            Update();
        }

        ~Object() override = default;

        void Update() override;
        void InitFromFile(const std::string& file) const;

        operator render::drawable::Drawable&() const { return *m_drawable; }

    private:
        Transform m_transform;
        std::unique_ptr<render::drawable::Drawable> m_drawable;
    };
}
