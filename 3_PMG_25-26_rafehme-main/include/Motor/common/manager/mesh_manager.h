#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__ 1

#include <memory>
#include <string>
#include <unordered_map>

class ModelAsset;

class MeshManager {
public:
	MeshManager() = default;
	~MeshManager() = default;

    std::shared_ptr<ModelAsset> Load(const std::string& path);

	const std::unordered_map<std::string, std::shared_ptr<ModelAsset>>& GetModels() {
		return models_;
	}

private:
    std::unordered_map<std::string,std::shared_ptr<ModelAsset>> models_;

};

#endif // !__MESH_MANAGER_H__
