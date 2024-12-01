#pragma once

#include <tiny_gltf.h>

#include "pch.h"

#include <wincodec.h>

#include "texture.hpp"

namespace x::fs
{
    class TextureLoader
    {
    public:
        static Texture LoadFromMemory(const BYTE* data, SIZE_T size);
        static Texture LoadFromModelIndex(const tinygltf::Model& model, const int index);

    private:
        static ComPtr<IWICImagingFactory> m_factory;

        static void m_Init();
    };
}
