#ifndef __MESH_COMPONENT_H__
#define __MESH_COMPONENT_H__ 1

#include <memory>
#include <future>
#include "Motor/common/asset/model_asset.h"
#include "Motor/common/dev/texture.h"
#include <nlohmann/json.hpp>

class MeshComponent {
public:
	MeshComponent();
	~MeshComponent();

	MeshComponent(const MeshComponent& other) = delete;
	MeshComponent& operator=(const MeshComponent& other) = delete;

	MeshComponent(MeshComponent&& other);
	MeshComponent& operator=(MeshComponent&& other);

	void SetModel(std::shared_ptr<ModelAsset> model) {
		model_ = model;
	}

	void SetFuture(std::shared_future<std::shared_ptr<ModelAsset>> future) {
		loading_future_ = future;
	}

	const std::vector<std::shared_ptr<MeshData>>& GetMeshes() {
		return model_->GetMeshes();
	}

	bool HasModel() {
		if (!model_)return false;

		return true;
	}

	const ModelAsset* GetModel() {
		return model_.get();
	}


	void Update();

	void Serialize(nlohmann::json& json);
	void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);



private:
	std::shared_ptr<ModelAsset> model_;
	std::shared_future<std::shared_ptr<ModelAsset>> loading_future_;
};

#endif