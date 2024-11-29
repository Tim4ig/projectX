#include "gltfLoader.hpp"

#include "tiny_gltf.h"

namespace x::fs
{
    std::unique_ptr<tinygltf::Model> gltfLoader::LoadModelFromFile(const std::string& path)
    {
        auto model = std::make_unique<tinygltf::Model>();
        std::string warn, err;

        if (!m_loader.LoadBinaryFromFile(model.get(), &warn, &err, path))
        {
            if (!warn.empty()) logger.Warning(PREFIX, warn);
            if (!err.empty()) logger.Error(PREFIX, err);
            return nullptr;
        }

        if (!warn.empty()) logger.Warning(PREFIX, warn);
        logger.Info(PREFIX, "Successfully loaded model from file: ", path);

        return model;
    }

    tinygltf::TinyGLTF gltfLoader::m_loader = tinygltf::TinyGLTF();
}
