#ifndef __MODEL_ASSET_H__ 
#define __MODEL_ASSET_H__ 1

#include <memory>
#include <vector>
#include <string>

class MeshData;
class Texture;

class ModelAsset {
public:
    ModelAsset() = default;
    ~ModelAsset() = default;

    void AddMesh(std::shared_ptr<MeshData> mesh);
    void AddTexture(Texture tex);
    
    void SetPath(std::string path)  {
        path_ = path;
    }

    std::string GetPath() const {
        return path_;
    }

    const std::vector<std::shared_ptr<MeshData>>& GetMeshes() const;
    const std::vector<Texture>& GetTextures() const;

private:
    std::vector<std::shared_ptr<MeshData>> meshes_;
    std::vector<Texture> textures_;
    std::string path_;
};

#endif // !__MODEL_ASSET_H__ 
