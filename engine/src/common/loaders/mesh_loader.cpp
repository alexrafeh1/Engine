#include "Motor/common/loaders/mesh_loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>
#include <GLFW/glfw3.h>


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
        return {};
    }

    ProcessNode(scene->mRootNode,scene,meshes.meshes,path);

    return meshes;
}


void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshSource>& meshes, const std::string& path)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh,scene,path));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, meshes,path);
    }
}

MeshSource MeshLoader::ProcessMesh(struct aiMesh* mesh, const aiScene* scene, const std::string& path) {
    MeshSource result;

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
            result.indices.push_back( face.mIndices[j] );
        }
    }

   aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
   aiString texturePath;

   std::filesystem::path modelDirectory = std::filesystem::path(path).parent_path();

   auto texture_fun = [&](aiTextureType type) -> std::optional<Texture> {
       aiString texturePath;

       if (mat->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
           return std::nullopt;

       std::string filename = texturePath.C_Str();

       if (!filename.empty() && filename[0] == '*') {
           const aiTexture* embedded =
               scene->GetEmbeddedTexture(filename.c_str());

           return Texture::LoadEmbeddedTexture(embedded);
       }

       std::filesystem::path textureFile(filename);

       if (!textureFile.is_absolute())
           textureFile = modelDirectory / textureFile;

       textureFile = std::filesystem::absolute(textureFile).lexically_normal();

       if (!std::filesystem::exists(textureFile)) {
           std::cerr << "Texture not found: "
               << textureFile.string() << '\n';
           return std::nullopt;
       }

       return Texture::LoadTexture(textureFile.string());
       };

   auto albedo = texture_fun(aiTextureType_BASE_COLOR);

   if (albedo) {
      result.textures.albedo = std::move(albedo);
   }
   else {
       result.textures.albedo = texture_fun(aiTextureType_DIFFUSE);
   }

   result.textures.normal = texture_fun(aiTextureType_NORMALS);
   result.textures.roughness = texture_fun(aiTextureType_DIFFUSE_ROUGHNESS);
   result.textures.metallic = texture_fun(aiTextureType_METALNESS);
   result.textures.ao = texture_fun(aiTextureType_AMBIENT_OCCLUSION);

   return result;
}