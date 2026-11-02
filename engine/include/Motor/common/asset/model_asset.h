#ifndef __MODEL_ASSET_H__ 
#define __MODEL_ASSET_H__ 1

#include <memory>
#include <vector>
#include <string>

#include "Motor/common/component_data/mesh_data.h"

class Texture;

class ModelAsset {
public:
    ModelAsset() = default;
    ~ModelAsset() = default;

    void AddMesh(std::shared_ptr<MeshData> mesh);
    
    void SetPath(std::string path)  {
        path_ = path;
    }

    std::string GetPath() const {
        return path_;
    }

    void CreateVAOs()
    {
        for (auto& mesh : meshes_)
        {
            mesh->CreateVAO();
        }
    }

    const std::vector<std::shared_ptr<MeshData>>& GetMeshes() const;
private:
    std::vector<std::shared_ptr<MeshData>> meshes_;
    std::string path_;
};

#endif // !__MODEL_ASSET_H__ 
