#include "pipeline_debug.hpp"

#include "settings.hpp"

namespace x::render::pipeline
{
    PipelineDebug::PipelineDebug(const ComPtr<ID3D11Device>& device) : IPipeline(device)
    {
        m_Init();
    }

    void PipelineDebug::Clear() const
    {
        IPipeline::Clear();
    }

    void PipelineDebug::Resize(const POINT size)
    {
        IPipeline::Resize(size);
    }

    void PipelineDebug::BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv)
    {
        IPipeline::BeginFrame(rtv, dsv);
    }

    ComPtr<ID3D11CommandList> PipelineDebug::EndFrame()
    {
        return IPipeline::EndFrame();
    }

    void PipelineDebug::Draw(const std::vector<const Drawable*>& queue) const
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

    void PipelineDebug::Bind(const drawable::Material& material) const
    {
        PipelineDebug::Bind(material.baseColorTexture, 0);
    }

    void PipelineDebug::Bind(const Shader& shader) const
    {
        IPipeline::Bind(shader);
    }

    void PipelineDebug::Bind(const ConstantBuffer& constantBuffer, const int slot) const
    {
        IPipeline::Bind(constantBuffer, slot);
    }

    void PipelineDebug::Bind(const Texture& texture, const int slot) const
    {
        IPipeline::Bind(texture, slot);
    }

    void PipelineDebug::Bind(Camera& camera)
    {
        IPipeline::Bind(camera);
    }

    void PipelineDebug::m_Init()
    {
        m_InitShader();
    }

    void PipelineDebug::m_InitShader()
    {
        m_shader = std::make_unique<Shader>(m_device);
        m_shader->Load(core::SHADER_PATH + L"debug.vs.cso", core::SHADER_PATH + L"debug.gs.cso", core::SHADER_PATH + L"debug.ps.cso", nullptr, 0);
    }
}
