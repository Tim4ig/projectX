#include "main_pipeline.hpp"

#include "settings.hpp"

namespace x::render::pipeline
{
    MainPipeline::MainPipeline(const ComPtr<ID3D11Device>& device) : m_device(device), m_constantBuffer(device)
    {
        m_Init();
    }

    void MainPipeline::Clear() const
    {
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void MainPipeline::Resize(const POINT size)
    {
        m_viewport.Width = static_cast<float>(size.x);
        m_viewport.Height = static_cast<float>(size.y);
    }

    void MainPipeline::BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv)
    {
        if (rtv == nullptr || dsv == nullptr)
            XTHROW("invalid render target view or depth stencil view");

        m_renderTargetView = rtv;
        m_depthStencilView = dsv;

        Clear();

        m_context->RSSetViewports(1, &m_viewport);
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

        Bind(*m_shader);
    }

    ComPtr<ID3D11CommandList> MainPipeline::EndFrame()
    {
        auto hr = S_OK;

        m_renderTargetView.Reset();
        m_depthStencilView.Reset();

        ComPtr<ID3D11CommandList> commandList;
        hr = m_context->FinishCommandList(FALSE, &commandList) HTHROW("failed to finish command list");

        return commandList;
    }

    void MainPipeline::Draw(const std::vector<const Drawable*>& queue) const
    {
        for (auto& drawable : queue)
        {
            std::function<void(const drawable::Node&)> drawNode = [&](const drawable::Node& node)
            {
                if (node.mesh != -1)
                {
                    Bind(node.constantBuffer, 1);

                    for (const auto& primitive : drawable->m_meshes[node.mesh].m_primitives)
                    {
                        Bind(primitive.m_material);

                        constexpr UINT offset = 0;
                        const auto stride = static_cast<UINT>(primitive.m_stride);

                        m_context->IASetVertexBuffers(0, 1, primitive.m_vertexBuffer.GetAddressOf(), &stride, &offset);
                        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                        m_context->Draw(primitive.m_vertexCount, 0);
                    }
                }

                for (const auto& child : node.children)
                    drawNode(child);
            };

            drawNode(drawable->m_root);
        }
    }

    void MainPipeline::Bind(const drawable::Material& material) const
    {
        Bind(material.baseColorTexture, 0);
    }

    void MainPipeline::Bind(const Shader& shader) const
    {
        m_context->IASetInputLayout(shader.GetInputLayout().Get());
        m_context->VSSetShader(shader.GetVertexShader().Get(), nullptr, 0);
        m_context->PSSetShader(shader.GetPixelShader().Get(), nullptr, 0);
    }

    void MainPipeline::Bind(const ConstantBuffer& constantBuffer, const int slot) const
    {
        if (constantBuffer.GetBuffer())
            m_context->VSSetConstantBuffers(slot, 1, constantBuffer.GetBuffer().GetAddressOf());
    }

    void MainPipeline::Bind(const Texture& texture, const int slot) const
    {
        if (texture.GetShaderResourceView())
            m_context->PSSetShaderResources(slot, 1, texture.GetShaderResourceView().GetAddressOf());
    }

    void MainPipeline::Bind(Camera& camera)
    {
        camera.UpdateProjectionMatrix(m_viewport.Width / m_viewport.Height, 0.25f * std::numbers::pi_v<float>);
        camera.UpdateViewProjectionMatrix();
        const auto viewProjectionMatrix = camera.GetViewProjectionMatrix();

        m_constantBuffer.SetConstantBuffer(&viewProjectionMatrix, sizeof(viewProjectionMatrix));
        Bind(m_constantBuffer, 0);
    }

    void MainPipeline::m_Init()
    {
        auto hr = S_OK;

        hr = m_device->CreateDeferredContext(0, &m_context) HTHROW("failed to create deferred context");

        m_viewport = {};
        m_viewport.Width = 800;
        m_viewport.Height = 600;
        m_viewport.MaxDepth = 1.0f;

        D3D11_SAMPLER_DESC sd = {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sd.MinLOD = 0;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        hr = m_device->CreateSamplerState(&sd, &m_samplerState) HTHROW("failed to create sampler state");

        m_InitShader();
    }

    void MainPipeline::m_InitShader()
    {
        m_shader = std::make_unique<Shader>(m_device);
        m_shader->Load(core::SHADER_PATH + L"test.vs.cso", core::SHADER_PATH + L"test.ps.cso", nullptr, 0);
    }
}
