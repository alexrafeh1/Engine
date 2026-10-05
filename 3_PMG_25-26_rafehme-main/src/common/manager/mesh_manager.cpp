#include "Motor/common/manager/mesh_manager.h"
#include "Motor/common/asset/model_asset.h"
#include "Motor/common/component_data/mesh_data.h"
#include "Motor/common/loaders/mesh_loader.h"



std::shared_ptr<ModelAsset> MeshManager::Load(const std::string& path)
{
    auto it = models_.find(path);

    if (it != models_.end())
    {
        return it->second;
    }

    ModelSource sources = MeshLoader::LoadMesh(path);

    if (sources.meshes.empty())
        return nullptr;

    auto model = std::make_shared<ModelAsset>();

    model->SetPath(sources.path);

    for (const MeshSource& source : sources.meshes)
    {
        auto mesh = MeshData::CreateMesh(source);

        if (!mesh)
            continue;

        auto meshPtr = std::make_shared<MeshData>(std::move(*mesh));
        
        model->AddMesh(meshPtr);
    }

    for (auto& texture : sources.textures)
    {
        model->AddTexture(std::move(texture));
    }

    models_[path] = model;

    return models_[path];

}
