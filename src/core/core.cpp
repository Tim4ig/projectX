
#include "core.hpp"

#include "logger.hpp"

namespace x::core
{
    Core::~Core()
    {
    }

    void Core::Init()
    {
        utils::Exception::SetMainThreadId();
        m_threadManager = ThreadManager::GetDefaultManager();
        m_window = std::make_unique<Window>();
        m_factory = std::make_unique<render::D3D11Factory>();

        m_window->OpenAsync(L"X Engine", 800, 600);
        m_renderer = m_factory->CreateRenderer(m_window->GetHandle());
    }

    void Core::StartLoop()
    {
        while (true)
        {
            if (!m_UserLoop()) break;
            if (!m_UserLoop()) break;
        }
    }

    bool Core::m_UserLoop()
    {
        if (!m_window->IsOpen()) return false;
        return true;
    }

    bool Core::m_SystemLoop()
    {
        m_threadManager->Update();
        return true;
    }
}
