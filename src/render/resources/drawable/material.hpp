#pragma once
#include "texture.hpp"

namespace x::render::drawable
{
    class Material
    {
    public:
        explicit Material(const ComPtr<ID3D11Device>& device) :
            baseColorTexture(device),
            metallicRoughnessTexture(device),
            normalTexture(device),
            occlusionTexturePath(device),
            emissiveTexture(device)
        {
        }

        std::string name;

        DirectX::XMFLOAT4 baseColorFactor = {1.0f, 1.0f, 1.0f, 1.0f};
        Texture baseColorTexture;

        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;
        Texture metallicRoughnessTexture;

        Texture normalTexture;
        float normalScale = 1.0f;

        Texture occlusionTexturePath;
        float occlusionStrength = 1.0f;

        DirectX::XMFLOAT3 emissiveFactor = {0.0f, 0.0f, 0.0f};
        Texture emissiveTexture;

        std::string alphaMode = "OPAQUE";
        float alphaCutoff = 0.5f;
        bool doubleSided = false;
    };
}
