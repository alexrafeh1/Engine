#include "Motor/common/components/mesh_component.h"
#include "Motor/common/manager/ecs_manager.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
}

MeshComponent::MeshComponent(MeshComponent&& other):
model_(std::move(other.model_))
{
}

MeshComponent& MeshComponent::operator=(MeshComponent&& other)
{
	model_ = std::move(other.model_);
	return *this;
}


void MeshComponent::Serialize(nlohmann::json& json) {
    json["MeshComponent"]["Model"] =  model_->GetPath() ;
}

void MeshComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {
	const auto& data = json["MeshComponent"];

	model_ = context.meshManager.Load(data["Model"]);
}