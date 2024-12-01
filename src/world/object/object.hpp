#pragma once

#include "render/resources/drawable/transform.hpp"
#include "render/resources/drawable/drawable.hpp"

namespace x::world
{
    class Object : public render::drawable::Transform
    {
    public:
        explicit Object(std::unique_ptr<render::Drawable>& drawable) : m_drawable(std::move(drawable))
        {
            Update();
        }

        ~Object() override = default;

        void Update() override;
        void InitFromFile(const std::string& file) const;

        operator render::Drawable*() const { return m_drawable.get(); }

    private:
        std::unique_ptr<render::Drawable> m_drawable;
    };
}
