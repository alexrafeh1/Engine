#ifndef __MATERIAL_COMPONENT_H__
#define __MATERIAL_COMPONENT_H__ 1

#include "Motor/common/component_data/material_data.h"
#include <nlohmann/json.hpp>

class MaterialComponent {
public:
	MaterialComponent() = default;
	~MaterialComponent() = default;

	MaterialComponent(const MaterialComponent& other) = default;
	MaterialComponent& operator=(const MaterialComponent& other) = default;

	MaterialComponent(MaterialComponent&&) noexcept = default;
	MaterialComponent& operator=(MaterialComponent&&) noexcept = default;

	MaterialData& GetMaterial() {
		return material_;
	}

	void Serialize(nlohmann::json& json);
	void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);


private:
	MaterialData material_;
};

#endif // !__MATERIAL_COMPONENT_H__
