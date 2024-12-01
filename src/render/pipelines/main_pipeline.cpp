#include "main_pipeline.hpp"

namespace x::render::pipeline
{
    void MainPipeline::Resize(const POINT size)
    {
        m_viewport.Width = static_cast<FLOAT>(size.x);
        m_viewport.Height = static_cast<FLOAT>(size.y);

        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_renderTargetView.Reset();
        m_depthStencilView.Reset();

        auto hr = S_OK;
        hr = m_swapChain->ResizeBuffers(0, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height), DXGI_FORMAT_UNKNOWN, 0) HTHROW("failed to resize buffers");

        m_InitBuffers();
    }

    void MainPipeline::SetClearColor(const unsigned int rgba)
    {
        m_clearColor[0] = static_cast<float>(rgba >> 24 & 0xFF) / 255.0f;
        m_clearColor[1] = static_cast<float>(rgba >> 16 & 0xFF) / 255.0f;
        m_clearColor[2] = static_cast<float>(rgba >> 8 & 0xFF) / 255.0f;
        m_clearColor[3] = static_cast<float>(rgba & 0xFF) / 255.0f;
    }

    void MainPipeline::Clear() const
    {
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void MainPipeline::BeginFrame()
    {
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
        m_context->RSSetViewports(1, &m_viewport);

        m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    }

    void MainPipeline::EndFrame()
    {
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

        m_InitBuffers();
    }

    void MainPipeline::m_InitBuffers()
    {
        auto hr = S_OK;

        ComPtr<ID3D11Texture2D> rtt;
        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&rtt)) HTHROW("failed to get back buffer");
        hr = m_device->CreateRenderTargetView(rtt.Get(), nullptr, &m_renderTargetView) HTHROW("failed to create render target view");

        {
            D3D11_TEXTURE2D_DESC dstd = {};
            dstd.Width = static_cast<UINT>(m_viewport.Width);
            dstd.Height = static_cast<UINT>(m_viewport.Height);
            dstd.MipLevels = 1;
            dstd.ArraySize = 1;
            dstd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dstd.SampleDesc.Count = 1;
            dstd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

            ComPtr<ID3D11Texture2D> dst;
            hr = m_device->CreateTexture2D(&dstd, nullptr, &dst) HTHROW("failed to create depth stencil texture");
            hr = m_device->CreateDepthStencilView(dst.Get(), nullptr, &m_depthStencilView) HTHROW("failed to create depth stencil view");
        }
    }
}
