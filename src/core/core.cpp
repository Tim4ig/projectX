#include "core.hpp"

#include "pch.h"

#include "develop/camera_controller.hpp"

#include "filesystem/gltf/gltfLoader.hpp"
#include "filesystem/gltf/gltfConvert.hpp"
#include "filesystem/textures/textureLoader.hpp"

namespace x::core
{
    Core::Core()
    {
    }

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
        m_renderer->SetResolution({1920, 1080});

        auto drawable = m_factory->CreateDrawable();
        m_drawableTest = std::unique_ptr<world::Object>(new world::Object(drawable));
        m_drawableTest->InitFromFile("../resources/a.glb");
        m_drawableTest->SetRotation(-1.6, 0, 0);
        m_drawableTest->Update();
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

        static render::Camera camera;
        static develop::CameraController cameraController(&camera);
        camera.UpdateProjectionMatrix(800.0f / 600.0f, 0.25f * std::numbers::pi_v<float>);

        cameraController.Update();

        m_renderer->BeginFrame();
        m_renderer->Clear();
        m_renderer->Bind(camera);
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
