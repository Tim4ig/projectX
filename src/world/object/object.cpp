#include "object.hpp"

#include "resources/drawable/mesh.hpp"

#include "filesystem/gltf/gltfLoader.hpp"
#include "filesystem/gltf/gltfConvert.hpp"
#include "resources/drawable/vertex.hpp"

void x::world::Object::InitFromFile(const std::string& file) const
{
    const auto model = fs::gltfLoader::LoadModelFromFile(file);

    for (const auto& mesh : model->meshes)
    {
        m_drawable->m_meshes.emplace_back(m_drawable->m_device);
        auto& myMesh = m_drawable->m_meshes.back();

        for (const auto& primitive : mesh.primitives)
        {
            myMesh.m_primitives.emplace_back(m_drawable->m_device);
            auto& myPrimitive = myMesh.m_primitives.back();

            std::vector<BYTE> vertices;
            fs::gltfConverter::PrimitiveToRaw(*model, primitive, vertices);

            myPrimitive.SetVertices(vertices.data(), vertices.size(), render::drawable::VERTEX_STRIDE);

            //TODO: load material to primitive
        }
    }

    std::function<render::drawable::Node(int)> nodeBuilder = [&](const int index) -> render::drawable::Node
    {
        render::drawable::Node myNode(m_drawable->m_device);
        const auto& node = model->nodes[index];

        myNode.name = node.name;
        myNode.mesh = node.mesh;

        {
            if (!node.translation.empty())
                myNode.transform.SetPosition(node.rotation[0], node.rotation[1], node.rotation[2]);

            if (!node.rotation.empty())
                myNode.transform.SetRotation(node.rotation[0], node.rotation[1], node.rotation[2]);

            if (!node.scale.empty())
                myNode.transform.SetScale(node.scale[0], node.scale[1], node.scale[2]);
        }

        for (const auto& child : model->nodes[index].children)
            myNode.children.emplace_back(nodeBuilder(child));

        return myNode;
    };

    m_drawable->m_root = nodeBuilder(0);
    m_drawable->m_root.transform.SetPosition(2, 1, 1);
    m_drawable->m_root.Update();
}

void x::world::Object::Update()
{
    m_transform.Update();
    const auto matrix = m_transform.GetWorldMatrix();
    m_drawable->m_root.Update(matrix);
}
