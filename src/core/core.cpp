
#include "core.hpp"

#include "logger.hpp"

namespace x::core
{
    void Core::Init()
    {
        m_window.OpenAsync(L"X Window", 800, 600);

        {
            m_factory = std::make_unique<render::D3D11Factory>();
            m_renderer = m_factory->CreateRenderer(m_window.GetHandle());
            m_renderer->SetClearColor(0xFF0000FF);
        }
    }

    void Core::StartLoop()
    {
        while (m_window.IsOpen())
        {
            static bool fs = false;
            static POINT p = { 800,600 };

            if (GetAsyncKeyState('A'))
            {
                fs = 1;
                m_renderer->SetResolution(p, fs);
            }

            if (GetAsyncKeyState('D'))
            {
                fs = 0;
                m_renderer->SetResolution(p, fs);
            }

            if (GetAsyncKeyState('S'))
            {
                p = { 1920, 1080 };
                m_renderer->SetResolution(p, fs);
            }

            m_renderer->BeginFrame();
            m_renderer->Clear();
            m_renderer->EndFrame();
        }
    }
}
