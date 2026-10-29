#include "Motor/common/manager/mesh_manager.h"
#include "Motor/common/asset/model_asset.h"
#include "Motor/common/dev/job_system.h"

#include <GLFW/glfw3.h>


MeshManager::MeshManager(JobSystem& job_system)
    : job_system_(job_system)
{
}

std::shared_future<std::shared_ptr<ModelAsset>> MeshManager::LoadAsync(const std::string& path)
{
    if (auto it = models_.find(path); it != models_.end())
    {
        std::promise<std::shared_ptr<ModelAsset>> promise;
        promise.set_value(it->second);
        return promise.get_future().share();
    }

    // Ya se está cargando.
    if (auto it = loading_.find(path); it != loading_.end())
    {
        return it->second;
    }

    // Carga CPU + recursos GPU desde el worker.
    auto future = job_system_.Submit(
        [path]()
        {
            ModelSource source = MeshLoader::LoadMesh(path);

            if (source.meshes.empty())
                return std::shared_ptr<ModelAsset>{};

            auto model = std::make_shared<ModelAsset>();

            model->SetPath(source.path);

            for (const MeshSource& meshSource : source.meshes)
            {
                auto mesh = MeshData::CreateMesh(meshSource);

                if (!mesh)
                    continue;

                auto meshPtr =
                    std::make_shared<MeshData>(
                        std::move(*mesh)
                    );

                model->AddMesh(meshPtr);
            }

            for (auto& texture : source.textures)
            {
                model->AddTexture(
                    std::move(texture)
                );
            }

            return model;
        }
    ).share();

    loading_[path] = future;

    return future;
}



std::shared_ptr<ModelAsset>MeshManager::Get(const std::string& path)
{
    auto it = models_.find(path);

    if (it == models_.end())
        return nullptr;

    return it->second;
}

bool MeshManager::IsLoaded(const std::string& path) const
{
    return models_.find(path) != models_.end();
}

bool MeshManager::IsLoading(const std::string& path) const
{
    return loading_.find(path) != loading_.end();
}