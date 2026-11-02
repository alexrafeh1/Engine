#include "Motor/common/components/mesh_component.h"
#include "Motor/common/manager/ecs_manager.h"

#include <chrono>

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
}

MeshComponent::MeshComponent(MeshComponent&& other):
model_(std::move(other.model_)), loading_future_(std::move(other.loading_future_))
{
}

MeshComponent& MeshComponent::operator=(MeshComponent&& other)
{
	model_ = std::move(other.model_);
    loading_future_ = std::move(other.loading_future_);
	return *this;
}

void MeshComponent::Update()
{
    if (!loading_future_.valid())
        return;

    if (loading_future_.wait_for(
        std::chrono::seconds(0))
        != std::future_status::ready)
    {
        return;
    }

    model_ = loading_future_.get();

    if (model_)
    {
        model_->CreateVAOs();
    }
}


void MeshComponent::Serialize(nlohmann::json& json) {
    json["MeshComponent"]["Model"] =  model_->GetPath() ;
}

void MeshComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {
    const auto& data = json["MeshComponent"];
    const std::string path = data["Model"];

    loading_future_ = context.meshManager.LoadAsync("../data/assets/scene.gltf");
}