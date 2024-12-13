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
        void Load(const std::wstring& vertexShaderPath, const std::wstring& geometryShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);

        ComPtr<ID3D11VertexShader> GetVertexShader() const { return m_vertexShader; }
        ComPtr<ID3D11GeometryShader> GetGeometryShader() const { return m_geometryShader; }
        ComPtr<ID3D11PixelShader> GetPixelShader() const { return m_pixelShader; }
        ComPtr<ID3D11InputLayout> GetInputLayout() const { return m_inputLayout; }

    private:
        ComPtr<ID3D11Device> m_device;

        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11GeometryShader> m_geometryShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        void m_Init(const std::wstring& vertexShaderPath, const std::wstring& geometryShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
        void m_InitInputLayout(const ComPtr<ID3DBlob>& vertexShaderBlob, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements);
    };
}
