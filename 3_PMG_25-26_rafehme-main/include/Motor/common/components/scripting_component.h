#ifndef __SCRIPTING_COMPONENT_H__
#define __SCRIPTING_COMPONENT_H__ 1 

#include "Motor/common/component_data/script_data.h"
#include <memory>
#include <nlohmann/json.hpp>

class ECSManager;

class ScriptingComponent {

public:
	ScriptingComponent() = default;
	~ScriptingComponent() = default;

	ScriptingComponent(ScriptingComponent&& other);
	ScriptingComponent& operator=(ScriptingComponent&& other);

	void SetScript(const std::string path, ECSManager& ecs,size_t entity) {
		script_ = std::make_shared<ScriptData>();
		script_->LoadScript(path, ecs, entity);
	}

	ScriptData* GetScript() {
		return script_.get();
	}

	void Serialize(nlohmann::json& json);
	void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);

private:
	std::shared_ptr<ScriptData> script_;
};

#endif // !__SCRIPTING_COMPONENT_H__
