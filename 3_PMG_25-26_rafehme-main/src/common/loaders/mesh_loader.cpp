#include "Motor/common/loaders/mesh_loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>

ModelSource MeshLoader::LoadMesh(const std::string& path)
{
    ModelSource meshes;

    meshes.path = path;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs
    );

    if (!scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        std::cerr
            << "Assimp error: "
            << importer.GetErrorString()
            << '\n';

        return {};
    }

    LoadTextures(scene,meshes,path);

    ProcessNode(
        scene->mRootNode,
        scene,
        meshes.meshes
    );

    return meshes;
}
void MeshLoader::LoadTextures(
    const aiScene* scene,
    ModelSource& model,
    const std::string& path)
{
    model.textures.resize(scene->mNumMaterials);

    std::filesystem::path modelPath(path); 
    std::filesystem::path modelDirectory = modelPath.parent_path();

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];

        aiString texturePath;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            std::filesystem::path textureFile = modelDirectory / texturePath.C_Str();

            textureFile = textureFile.lexically_normal();

            std::cout
                << "Texture: "
                << textureFile.string()
                << '\n';

            auto tex = Texture::LoadTexture(
                textureFile.string()
            );

            if (tex)
            {
                model.textures[i] = std::move(tex.value());
            }
        }
    }
}



void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshSource>& meshes)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(
            ProcessMesh(mesh)
        );
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(
            node->mChildren[i],
            scene,
            meshes
        );
    }
}

MeshSource MeshLoader::ProcessMesh(struct aiMesh* mesh) {
    MeshSource result;


    result.textureIndex = mesh->mMaterialIndex;

    result.vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex{};

        // Position
        vertex.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        // Normals
        if (mesh->HasNormals())
        {
            vertex.normals = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }

        // UVs
        if (mesh->mTextureCoords[0])
        {
            vertex.uv = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }

        vertex.tangent = { 1.0f, 1.0f, 1.0f };

        result.vertices.push_back(vertex);
    }


    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            result.indices.push_back(
                face.mIndices[j]
            );
        }
    }

    return result;
}