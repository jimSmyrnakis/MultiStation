#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <iostream>
#include <sstream>



namespace MultiStation {


    std::vector<MultiStation::MeshData> LoadFromAssimpModel(const std::string& path)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_TransformUVCoords |
            aiProcess_GenNormals |
            aiProcess_ImproveCacheLocality |
            aiProcess_OptimizeMeshes
        );

        if (!scene || !scene->mRootNode)
        {
            std::stringstream ss;
            ss << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
            ASSERT(0, ss.str().c_str());
            return {};
        }

        std::vector<MultiStation::MeshData> meshes;

        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];

            MultiStation::MeshData data{};
            data.vertexCount = mesh->mNumVertices;

            // allocate arrays
            data.positions = new float[data.vertexCount * 3];
            data.normals = new float[data.vertexCount * 3];
            data.texCords = new float[data.vertexCount * 2];
            // ---- copy vertex data ----
            for (unsigned int i = 0; i < data.vertexCount; i++)
            {
                data.positions[i * 3 + 0] = mesh->mVertices[i].x;
                data.positions[i * 3 + 1] = mesh->mVertices[i].y;
                data.positions[i * 3 + 2] = mesh->mVertices[i].z;

                data.normals[i * 3 + 0] = mesh->mNormals[i].x;
                data.normals[i * 3 + 1] = mesh->mNormals[i].y;
                data.normals[i * 3 + 2] = mesh->mNormals[i].z;

                if (mesh->mTextureCoords[0]) {
                    data.texCords[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
                    data.texCords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
                }
                else {
                    data.texCords[i * 2 + 0] = 0.0f;
                    data.texCords[i * 2 + 1] = 0.0f;
                }
            }

            // ---- copy indices ----
            data.indexCount = mesh->mNumFaces * 3; // λόγω Triangulate
            data.indices = new uint32_t[data.indexCount];

            uint32_t indexOffset = 0;
            for (unsigned int f = 0; f < mesh->mNumFaces; f++)
            {
                const aiFace& face = mesh->mFaces[f];

                // κάθε face έχει 3 indices λόγω Triangulate
                data.indices[indexOffset + 0] = face.mIndices[0];
                data.indices[indexOffset + 1] = face.mIndices[1];
                data.indices[indexOffset + 2] = face.mIndices[2];

                indexOffset += 3;
            }

            meshes.push_back(data);
        }

        return meshes;
    }

    void Model::LoadModel(std::string& path) {
        DestroyMesh();
        m_Meshes = LoadFromAssimpModel(path);
        Load();

    }

    void Model::DestroyMesh(void) {
        for (size_t i = 0; i < m_Meshes.size(); i++) {

            delete[] m_Meshes[i].positions;
            delete[] m_Meshes[i].normals;
            delete[] m_Meshes[i].texCords;
            delete[] m_Meshes[i].indices;

            
        }

        m_Meshes.clear();
        delete[] m_VBuffers;
        m_VBuffers = nullptr;
        m_Layouts.clear();
    }

    void Model::Load()
    {
        size_t count = m_Meshes.size();
        m_VBuffers = new VBuffer[count];

        for (size_t m = 0; m < count; m++)
        {
            auto& mesh = m_Meshes[m];

            // --- Φτιάχνουμε interleaved vertex buffer ---
            struct Vertex
            {
                float pos[3];
                float normal[3];
                float texCord[2];
            };

            std::vector<Vertex> vertices(mesh.vertexCount);

            for (uint32_t i = 0; i < mesh.vertexCount; i++)
            {
                vertices[i].pos[0] = mesh.positions[i * 3 + 0];
                vertices[i].pos[1] = mesh.positions[i * 3 + 1];
                vertices[i].pos[2] = mesh.positions[i * 3 + 2];

                vertices[i].normal[0] = mesh.normals[i * 3 + 0];
                vertices[i].normal[1] = mesh.normals[i * 3 + 1];
                vertices[i].normal[2] = mesh.normals[i * 3 + 2];

                vertices[i].texCord[0] = mesh.texCords[i * 2 + 0];
                vertices[i].texCord[1] = mesh.texCords[i * 2 + 1];
            }

            VertexLayout layout;
            layout.AddAttribute(VertexAttribute(ShaderDataType::VEC3F, false, true)); // position
            layout.AddAttribute(VertexAttribute(ShaderDataType::VEC3F, false, true)); // normal
            layout.AddAttribute(VertexAttribute(ShaderDataType::VEC2F, false, true)); // text coordinate
            m_Layouts.push_back(layout);

            VBuffer& vb = m_VBuffers[m];
            vb.SetLayout(layout);

            vb.SetData(vertices.data(), vertices.size() * sizeof(Vertex), 0);

            // index buffer
            IBufferAttrib iba;
            iba.Count = mesh.indexCount;
            iba.TypeSize = sizeof(uint32_t);
            iba.Signed = false;

            vb.SetIndexBuffer(mesh.indices, iba);
        }
    }

    VBuffer* Model::GetVBuffers(void) const {
        return m_VBuffers;
    }

    uint32_t  Model::GetVBuffersCount(void) const {
        return m_Meshes.size();
    }

}
