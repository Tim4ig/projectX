#include "main_pipeline.hpp"

#include "settings.hpp"

namespace x::render::pipeline
{
    void MainPipeline::BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv)
    {
        if (rtv == nullptr || dsv == nullptr)
            XTHROW("invalid render target view or depth stencil view");

        m_renderTargetView = rtv;
        m_depthStencilView = dsv;

        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
        Bind(*m_shader);
    }

    void MainPipeline::EndFrame()
    {
        m_renderTargetView.Reset();
        m_depthStencilView.Reset();
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
