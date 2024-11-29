#include "gltfConvert.hpp"

namespace x::fs
{
    struct Vertex {
        float position[3]; // x, y, z
        float texCoord[2]; // u, v
        float normal[3];   // nx, ny, nz
    };

    void gltfConverter::ModelToRawVertexData(tinygltf::Model& model, std::vector<BYTE>& raw)
    {
        // Store the final vertices
        std::vector<Vertex> vertices;

        for (const auto& mesh : model.meshes) {
            for (const auto& primitive : mesh.primitives) {
                // Extract POSITION
                const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
                const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];

                const unsigned char* posData = posBuffer.data.data() + posBufferView.byteOffset + posAccessor.byteOffset;
                size_t vertexCount = posAccessor.count;
                size_t posStride = posAccessor.ByteStride(posBufferView);

                // Extract TEXCOORD_0 if available
                const tinygltf::Accessor* texAccessor = nullptr;
                const unsigned char* texData = nullptr;
                size_t texStride = 0;
                if (primitive.attributes.contains("TEXCOORD_0")) {
                    texAccessor = &model.accessors[primitive.attributes.at("TEXCOORD_0")];
                    const tinygltf::BufferView& texBufferView = model.bufferViews[texAccessor->bufferView];
                    const tinygltf::Buffer& texBuffer = model.buffers[texBufferView.buffer];

                    texData = texBuffer.data.data() + texBufferView.byteOffset + texAccessor->byteOffset;
                    texStride = texAccessor->ByteStride(texBufferView);
                }

                // Extract NORMAL if available
                const tinygltf::Accessor* normAccessor = nullptr;
                const unsigned char* normData = nullptr;
                size_t normStride = 0;
                if (primitive.attributes.contains("NORMAL")) {
                    normAccessor = &model.accessors[primitive.attributes.at("NORMAL")];
                    const tinygltf::BufferView& normBufferView = model.bufferViews[normAccessor->bufferView];
                    const tinygltf::Buffer& normBuffer = model.buffers[normBufferView.buffer];

                    normData = normBuffer.data.data() + normBufferView.byteOffset + normAccessor->byteOffset;
                    normStride = normAccessor->ByteStride(normBufferView);
                }

                // Handle indices if present
                if (primitive.indices >= 0) {
                    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
                    const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
                    const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

                    const unsigned char* indexData = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;

                    for (size_t i = 0; i < indexAccessor.count; ++i) {
                        // Get the index (convert to uint32_t for safety)
                        uint32_t index = 0;
                        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                            index = reinterpret_cast<const uint16_t*>(indexData)[i];
                        }
                        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                            index = reinterpret_cast<const uint32_t*>(indexData)[i];
                        }
                        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                            index = reinterpret_cast<const uint8_t*>(indexData)[i];
                        }

                        // Build the vertex from indexed data
                        Vertex vertex = {};

                        // Position
                        const float* pos = reinterpret_cast<const float*>(posData + index * posStride);
                        vertex.position[0] = pos[0];
                        vertex.position[1] = pos[1];
                        vertex.position[2] = pos[2];

                        // Texture Coordinate
                        if (texAccessor) {
                            const float* tex = reinterpret_cast<const float*>(texData + index * texStride);
                            vertex.texCoord[0] = tex[0];
                            vertex.texCoord[1] = tex[1];
                        }

                        // Normal
                        if (normAccessor) {
                            const float* norm = reinterpret_cast<const float*>(normData + index * normStride);
                            vertex.normal[0] = norm[0];
                            vertex.normal[1] = norm[1];
                            vertex.normal[2] = norm[2];
                        }

                        vertices.push_back(vertex);
                    }
                }
                else {
                    // If no indices, process vertices sequentially
                    for (size_t i = 0; i < vertexCount; ++i) {
                        Vertex vertex = {};

                        // Position
                        const float* pos = reinterpret_cast<const float*>(posData + i * posStride);
                        vertex.position[0] = pos[0];
                        vertex.position[1] = pos[1];
                        vertex.position[2] = pos[2];

                        // Texture Coordinate
                        if (texAccessor) {
                            const float* tex = reinterpret_cast<const float*>(texData + i * texStride);
                            vertex.texCoord[0] = tex[0];
                            vertex.texCoord[1] = tex[1];
                        }

                        // Normal
                        if (normAccessor) {
                            const float* norm = reinterpret_cast<const float*>(normData + i * normStride);
                            vertex.normal[0] = norm[0];
                            vertex.normal[1] = norm[1];
                            vertex.normal[2] = norm[2];
                        }

                        vertices.push_back(vertex);
                    }
                }
            }
        }

        raw.resize(vertices.size() * sizeof(Vertex));
        memcpy(raw.data(), vertices.data(), raw.size());
    }
}
