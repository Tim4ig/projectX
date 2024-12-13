#pragma once

#include "pipeline.hpp"

namespace x::render::pipeline
{
    class PipelineDebug : public IPipeline
    {
    public:
        explicit PipelineDebug(const ComPtr<ID3D11Device>& device);
        ~PipelineDebug() override = default;

        void Resize(POINT size) override;

        void Clear() const override;

        void BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv) override;
        ComPtr<ID3D11CommandList> EndFrame() override;

        void Draw(const std::vector<const Drawable*>& queue) const override;

        void Bind(const drawable::Material& material) const override;
        void Bind(const Shader& shader) const override;
        void Bind(const ConstantBuffer& constantBuffer, int slot) const override;
        void Bind(const Texture& texture, int slot) const override;
        void Bind(Camera& camera) override;

    private:
        void m_Init() override;
        void m_InitShader() override;
    };
}
