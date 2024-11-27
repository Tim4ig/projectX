#pragma once

#include "pch.h"

#include "window.hpp"
#include "render/d3d11factory.hpp"
#include "render/d3d11renderer.hpp"
#include "render/resources/d3d11drawable.hpp"

namespace x::core
{
    class Core
    {
    public:
        ~Core();

        void Init();
        void StartLoop();

    private:
        std::unique_ptr<Window> m_window;
        ThreadManager* m_threadManager = nullptr;

        std::unique_ptr<render::Factory> m_factory;
        std::unique_ptr<render::Renderer> m_renderer;

        bool m_UserLoop();
        bool m_SystemLoop();

        std::unique_ptr<render::Drawable> m_drawableTest;
        std::unique_ptr<render::Shader> m_shaderTest;
    };
}
