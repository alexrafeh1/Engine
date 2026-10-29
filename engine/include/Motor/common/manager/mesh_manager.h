#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__ 1

#include <memory>
#include <string>
#include <unordered_map>
#include <future>
#include "Motor/common/component_data/mesh_data.h"
#include "Motor/common/loaders/mesh_loader.h"


class ModelAsset;
class JobSystem;

class MeshManager
{
public:
    explicit MeshManager(JobSystem& job_system);

    std::shared_future<std::shared_ptr<ModelAsset>> LoadAsync(const std::string& path);

    std::shared_ptr<ModelAsset> Get(const std::string& path);

    bool IsLoaded(const std::string& path) const;
    bool IsLoading(const std::string& path) const;

private:

private:
    JobSystem& job_system_;

    std::unordered_map<std::string,std::shared_ptr<ModelAsset>> models_;
    std::unordered_map<std::string,std::shared_future<std::shared_ptr<ModelAsset>>> loading_;
};

#endif // !__MESH_MANAGER_H__
