#include "textureLoader.hpp"

namespace x::fs
{
    Texture TextureLoader::LoadFromMemory(const BYTE* data, const SIZE_T size)
    {
        auto hr = S_OK;

        Texture texture;

        // tinygltf load image automatically using stb_image so we don't need to use WIC
        if (false)
        {
            if (!m_factory) m_Init();
            ComPtr<IWICStream> stream;
            ComPtr<IWICBitmapDecoder> decoder;
            ComPtr<IWICBitmapFrameDecode> frame;
            ComPtr<IWICFormatConverter> converter;

            hr = m_factory->CreateStream(&stream) HTHROW("Failed to create WIC stream");
            hr = stream->InitializeFromMemory(const_cast<BYTE*>(data), static_cast<DWORD>(size)) HTHROW("Failed to initialize WIC stream");
            hr = m_factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, &decoder)
            HTHROW("Failed to create WIC decoder");
            hr = decoder->GetFrame(0, &frame) HTHROW("Failed to get WIC frame");
            hr = m_factory->CreateFormatConverter(&converter) HTHROW("Failed to create WIC format converter");
            hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom) HTHROW("Failed to initialize WIC format converter");

            hr = converter->GetSize(&texture.height, &texture.height) HTHROW("Failed to get WIC size");

            texture.bufferSize = texture.width * texture.height * 4;
            texture.data = std::make_unique<BYTE[]>(texture.bufferSize);

            hr = converter->CopyPixels(nullptr, texture.width * 4, texture.bufferSize, texture.data.get()) HTHROW("Failed to copy WIC pixels");
        }
        else
        {
            texture.bufferSize = size;
            texture.data = std::make_unique<BYTE[]>(size);
            memcpy(texture.data.get(), data, size);
        }

        return texture;
    }

    void TextureLoader::m_Init()
    {
        const auto hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_factory)) HTHROW("Failed to create WIC factory");
    }

    ComPtr<IWICImagingFactory> TextureLoader::m_factory;
}
