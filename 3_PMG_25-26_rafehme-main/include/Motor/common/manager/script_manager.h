#ifndef __SCRIPT_MANAGER_H__ 
#define __SCRIPT_MANAGER_H__ 1

#include <memory>
#include <string>
#include <unordered_map>

class ScriptData;

class MeshManager {
public:
	MeshManager() = default;
	~MeshManager() = default;

	std::shared_ptr<ScriptData> Load(const std::string& path);

	const std::unordered_map<std::string, std::shared_ptr<ScriptData>>& GetModels() {
		return models_;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<ScriptData>> models_;
};

#endif // !__SCRIPT_MANAGER_H__