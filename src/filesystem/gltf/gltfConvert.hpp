#pragma once

#include "tiny_gltf.h"

namespace x::fs
{
    class gltfConverter
    {
    public:
        static void ModelToRawVertexData(tinygltf::Model& model, std::vector<BYTE>& raw);
    };
}
