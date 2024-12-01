#pragma once

#include "transform.hpp"
#include "resources/constant.hpp"

namespace x::render::drawable
{
    class Node
    {
    public:
        explicit Node(const ComPtr<ID3D11Device>& device) : constantBuffer(device)
        {
        }

        std::string name;
        int mesh = -1;
        Transform transform;
        std::vector<Node> children;
        ConstantBuffer constantBuffer;

        void Update()
        {
            Update(DirectX::XMMatrixIdentity());
        }

        void Update(const DirectX::XMMATRIX& parent)
        {
            transform.Update(parent);
            const auto matrix = transform.GetWorldMatrix();
            constantBuffer.SetConstantBuffer(&matrix, sizeof(matrix));

            for (auto& child : children)
                child.Update(matrix);
        }
    };
}
