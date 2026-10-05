#include "Motor/common/components/scripting_component.h"
#include "Motor/common/manager/ecs_manager.h"

ScriptingComponent::ScriptingComponent(ScriptingComponent&& other):
	script_(std::move(other.script_))
{

}

ScriptingComponent& ScriptingComponent::operator=(ScriptingComponent&& other)
{
	script_ = std::move(other.script_);

	return *this;
}


void ScriptingComponent::Serialize(nlohmann::json& json) {
	if (!script_)return;
	json["ScriptingComponent"]["Script"] = script_->GetPath();
	json["ScriptingComponent"]["Entity"] = script_->GetEntity();

}
void ScriptingComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {
	script_->LoadScript(json["ScriptingComponent"]["Script"], context.ecs, json["ScriptingComponent"]["Entity"]);
}