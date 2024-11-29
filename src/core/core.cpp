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
        const auto hr = CoInitialize(nullptr) HTHROW("Failed to initialize COM");
    }

    Core::~Core()
    {
        std::atexit([] { CoUninitialize(); });
    }

    void Core::Init()
    {
        utils::Exception::SetMainThreadId();
        m_threadManager = ThreadManager::GetDefaultManager();
        m_window = std::make_unique<Window>();
        m_factory = std::make_unique<render::Factory>();

        m_window->OpenAsync(L"X Engine", 800, 600);
        m_renderer = m_factory->CreateRenderer(m_window->GetHandle());
        m_renderer->SetResolution({ 1920,1080 });

        auto drawable = m_factory->CreateDrawable();
        m_drawableTest = std::unique_ptr<world::Object>(new world::Object(drawable));
        m_shaderTest = m_factory->CreateShader();

        m_shaderTest->Load(L"Debug/test.vs.cso", L"Debug/test.ps.cso", nullptr, 0);

        std::vector<BYTE> vertices;
        const auto box = fs::gltfLoader::LoadModelFromFile("../resources/untitled.glb");
        fs::gltfConverter::ModelToRawVertexData(*box, vertices);
        constexpr auto stride = (3 + 3 + 2) * sizeof(float);

        auto testT = fs::TextureLoader::LoadFromMemory(box->images[box->textures[0].source].image.data(), box->images[box->textures[0].source].image.size());
        testT.width = box->images[box->textures[0].source].width;
        testT.height = box->images[box->textures[0].source].height;

        m_drawableTest->m_SetVertecis(vertices.data(), vertices.size() / stride, vertices.size());
        m_drawableTest->m_SetTexture(testT);
        DirectX::XMFLOAT4 rot;
        DirectX::XMStoreFloat4(&rot, DirectX::XMVectorSet(3.14f, 0, 0, 0));
        m_drawableTest->SetRotation(rot);
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
        camera.UpdateProjectionMatrix(800.0f / 600.0f, 0.25f * 3.14159265359f);

        cameraController.Update();

        m_renderer->BeginFrame();
        m_renderer->Clear();
        m_renderer->Bind(camera);
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
