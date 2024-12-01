#pragma once

namespace x::fs
{
    struct Texture
    {
        UINT bufferSize = 0;
        std::unique_ptr<BYTE[]> data;
        UINT width = 0;
        UINT height = 0;
    };
}
