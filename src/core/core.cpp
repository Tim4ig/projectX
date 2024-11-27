#include "core.hpp"

#include "pch.h"

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
        m_factory = std::make_unique<render::Factory>();

        m_window->OpenAsync(L"X Engine", 800, 600);
        m_renderer = m_factory->CreateRenderer(m_window->GetHandle());

        m_drawableTest = m_factory->CreateObject();
        m_shaderTest = m_factory->CreateShader();

        m_shaderTest->Load(L"Debug/test.vs.cso", L"Debug/test.ps.cso", nullptr, 0);

        constexpr float vertices[] =
        {
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
        };

        m_drawableTest->SetVertecis(vertices, 3, sizeof(float) * 3 * 3);
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

        if (GetAsyncKeyState(VK_SPACE))
            m_renderer->SetResolution({2560, 1440});

        if (GetAsyncKeyState(VK_RETURN))
            m_renderer->SetResolution({800, 600});

        m_renderer->BeginFrame();
        m_renderer->Clear();
        m_renderer->Bind(*m_shaderTest);
        m_renderer->Draw(*m_drawableTest);
        m_renderer->EndFrame();

        return true;
    }

    bool Core::m_SystemLoop()
    {
        m_threadManager->Update();
        return true;
    }
}
