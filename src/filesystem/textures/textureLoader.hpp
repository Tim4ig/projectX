#pragma once

#include "pch.h"

#include <wincodec.h>

#include "textureT.hpp"

namespace x::fs
{
    class TextureLoader
    {
    public:
        static Texture LoadFromMemory(const BYTE* data, SIZE_T size);

    private:
        static ComPtr<IWICImagingFactory> m_factory;

        static void m_Init();
    };
}
