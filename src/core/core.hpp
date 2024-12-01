#pragma once

#include "window.hpp"
#include "render/factory.hpp"
#include "render/renderer.hpp"
#include "world/object/object.hpp"

namespace x::core
{
    class Core
    {
    public:
        Core();
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

        std::unique_ptr<world::Object> m_drawableTest;
    };
}
