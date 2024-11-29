#pragma once

#include "tiny_gltf.h"

namespace x::fs
{
    class gltfLoader
    {
    public:
        static std::unique_ptr<tinygltf::Model> LoadModelFromFile(const std::string& path);
    private:
        static tinygltf::TinyGLTF m_loader;
    };
}
