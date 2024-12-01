#include "gltfConvert.hpp"

namespace x::fs
{
    struct Vertex
    {
        float position[3]; // x, y, z
        float texCoord[2]; // u, v
        float normal[3];   // nx, ny, nz
    };

    void gltfConverter::PrimitiveToRaw(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::vector<BYTE>& raw)
    {
        std::vector<Vertex> vertices;

        const tinygltf::Accessor* posAccessor = &model.accessors[primitive.attributes.at("POSITION")];
        const tinygltf::Accessor* texAccessor = nullptr;
        const tinygltf::Accessor* normAccessor = nullptr;

        const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor->bufferView];
        const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];

        const auto posData = posBuffer.data.data() + posBufferView.byteOffset + posAccessor->byteOffset;
        const unsigned char* texData = nullptr;
        const unsigned char* normData = nullptr;

        const auto vertexCount = posAccessor->count;

        const auto posStride = posAccessor->ByteStride(posBufferView);
        size_t texStride = 0;
        size_t normStride = 0;

        if (primitive.attributes.contains("TEXCOORD_0"))
        {
            texAccessor = &model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const tinygltf::BufferView& texBufferView = model.bufferViews[texAccessor->bufferView];
            const tinygltf::Buffer& texBuffer = model.buffers[texBufferView.buffer];

            texData = texBuffer.data.data() + texBufferView.byteOffset + texAccessor->byteOffset;
            texStride = texAccessor->ByteStride(texBufferView);
        }

        if (primitive.attributes.contains("NORMAL"))
        {
            normAccessor = &model.accessors[primitive.attributes.at("NORMAL")];
            const tinygltf::BufferView& normBufferView = model.bufferViews[normAccessor->bufferView];
            const tinygltf::Buffer& normBuffer = model.buffers[normBufferView.buffer];

            normData = normBuffer.data.data() + normBufferView.byteOffset + normAccessor->byteOffset;
            normStride = normAccessor->ByteStride(normBufferView);
        }

        if (primitive.indices >= 0)
        {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

            const unsigned char* indexData = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;

            for (size_t i = 0; i < indexAccessor.count; ++i)
            {
                uint32_t index = 0;
                if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                    index = reinterpret_cast<const uint16_t*>(indexData)[i];
                else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                    index = reinterpret_cast<const uint32_t*>(indexData)[i];
                else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
                    index = reinterpret_cast<const uint8_t*>(indexData)[i];

                Vertex vertex = {};

                const auto pos = reinterpret_cast<const float*>(posData + index * posStride);
                vertex.position[0] = pos[0];
                vertex.position[1] = pos[1];
                vertex.position[2] = pos[2];

                if (texAccessor)
                {
                    const auto tex = reinterpret_cast<const float*>(texData + index * texStride);
                    vertex.texCoord[0] = tex[0];
                    vertex.texCoord[1] = tex[1];
                }

                if (normAccessor)
                {
                    const auto norm = reinterpret_cast<const float*>(normData + index * normStride);
                    vertex.normal[0] = norm[0];
                    vertex.normal[1] = norm[1];
                    vertex.normal[2] = norm[2];
                }

                vertices.push_back(vertex);
            }
        }
        else
        {
            for (size_t i = 0; i < vertexCount; ++i)
            {
                Vertex vertex = {};

                const auto pos = reinterpret_cast<const float*>(posData + i * posStride);
                vertex.position[0] = pos[0];
                vertex.position[1] = pos[1];
                vertex.position[2] = pos[2];

                if (texAccessor)
                {
                    const auto tex = reinterpret_cast<const float*>(texData + i * texStride);
                    vertex.texCoord[0] = tex[0];
                    vertex.texCoord[1] = tex[1];
                }

                if (normAccessor)
                {
                    const auto norm = reinterpret_cast<const float*>(normData + i * normStride);
                    vertex.normal[0] = norm[0];
                    vertex.normal[1] = norm[1];
                    vertex.normal[2] = norm[2];
                }

                vertices.push_back(vertex);
            }
        }

        raw.resize(vertices.size() * sizeof(Vertex));
        memcpy(raw.data(), vertices.data(), raw.size());
    }
}
