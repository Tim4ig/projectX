#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Shader
    {
    public:
        explicit Shader(const ComPtr<ID3D11Device>& device);

        void Load(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);

    private:
        ComPtr<ID3D11Device> m_device;

        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        void m_Init(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
        void m_InitInputLayout(const ComPtr<ID3DBlob>& vertexShaderBlob, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);

        friend class Renderer;
    };
}
