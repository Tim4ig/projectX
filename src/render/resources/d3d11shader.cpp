#include "d3d11shader.hpp"

#include <d3dcompiler.h>

#include "pch.h"

namespace x::render
{
    Shader::Shader(const ComPtr<ID3D11Device>& device) : m_device(device)
    {
    }

    void Shader::Load(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements)
    {
        m_Init(vertexShaderPath, pixelShaderPath, inputLayout, numElements);
    }

    void Shader::m_Init(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements)
    {
        auto hr = S_OK;
        ComPtr<ID3DBlob> vertexShaderBlob;
        ComPtr<ID3DBlob> pixelShaderBlob;

        hr = D3DReadFileToBlob(vertexShaderPath.c_str(), &vertexShaderBlob) HTHROW("Failed to read vertex shader file");
        hr = D3DReadFileToBlob(pixelShaderPath.c_str(), &pixelShaderBlob) HTHROW("Failed to read pixel shader file");

        hr = m_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &m_vertexShader) HTHROW("Failed to create vertex shader");
        hr = m_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &m_pixelShader) HTHROW("Failed to create pixel shader");

        m_InitInputLayout(vertexShaderBlob, inputLayout, numElements);
    }

    void Shader::m_InitInputLayout(const ComPtr<ID3DBlob>& vertexShaderBlob, D3D11_INPUT_ELEMENT_DESC* inputLayout, const UINT numElements)
    {
        auto hr = S_OK;

        if (inputLayout == nullptr || numElements == 0)
        {
            ComPtr<ID3D11ShaderReflection> vertexShaderReflection;
            hr = D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_PPV_ARGS(&vertexShaderReflection)) HTHROW("Failed to reflect vertex shader");

            D3D11_SHADER_DESC shaderDesc;
            hr = vertexShaderReflection->GetDesc(&shaderDesc) HTHROW("Failed to get shader description");

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
            for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
            {
                D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
                hr = vertexShaderReflection->GetInputParameterDesc(i, &paramDesc) HTHROW("Failed to get input parameter description");

                D3D11_INPUT_ELEMENT_DESC elementDesc;
                elementDesc.SemanticName = paramDesc.SemanticName;
                elementDesc.SemanticIndex = paramDesc.SemanticIndex;
                elementDesc.InputSlot = 0;
                elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                elementDesc.InstanceDataStepRate = 0;

                if (paramDesc.Mask == 1)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                }
                else if (paramDesc.Mask <= 3)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                }
                else if (paramDesc.Mask <= 7)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                else if (paramDesc.Mask <= 15)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                }

                inputLayoutDesc.emplace_back(elementDesc);
            }

            hr = m_device->CreateInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_inputLayout) HTHROW("Failed to create input layout");
        }
        else
        {
            hr = m_device->CreateInputLayout(
                inputLayout,
                numElements,
                vertexShaderBlob->GetBufferPointer(),
                vertexShaderBlob->GetBufferSize(),
                &m_inputLayout
            ) HTHROW("Failed to create input layout");
        }
    }
}
