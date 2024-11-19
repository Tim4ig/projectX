
#pragma once

#include <memory>

#include "exception.hpp"
#include "window.hpp"

#include "render/d3d11factory.hpp"
#include "render/d3d11renderer.hpp"

namespace x::core
{
    class Core
    {
    public:
        void Init();
        void StartLoop();
    private:
        Window m_window;
        std::unique_ptr<render::D3D11Factory> m_factory;
        std::unique_ptr<render::D3D11Renderer> m_renderer;
    };
}
