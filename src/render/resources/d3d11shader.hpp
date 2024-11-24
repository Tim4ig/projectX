
#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class D3D11Shader
    {
    public:
        D3D11Shader(const ComPtr<ID3D11Device>& device, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
        ~D3D11Shader();

        void Bind(const ComPtr<ID3D11DeviceContext>& context) const;
    private:
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        void m_Init(const ComPtr<ID3D11Device>& device, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
        void m_InitInputLayout(const ComPtr<ID3D11Device>& device, const ComPtr<ID3DBlob>& vertexShaderBlob, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
    };
}
