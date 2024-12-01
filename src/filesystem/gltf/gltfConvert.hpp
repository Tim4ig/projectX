#pragma once

#include "tiny_gltf.h"

namespace x::fs
{
    class gltfConverter
    {
    public:
        static void PrimitiveToRaw(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::vector<BYTE>& raw);
    };
}
