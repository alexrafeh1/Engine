#include "Motor/common/components/material_component.h"
#include "Motor/common/manager/ecs_manager.h"



void MaterialComponent::Serialize(nlohmann::json& json) {

    glm::vec4 color = material_.GetColor();
    json["MaterialComponent"]["Color"] = { color.x,color.y,color.z,color.w};
    json["MaterialComponent"]["Roughness"] = material_.GetRoughness();
}

void MaterialComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {

    const auto& data = json["MaterialComponent"];

    const auto& color_data = data["Color"];

    glm::vec4 color = material_.GetColor();

    color.x = color_data[0];
    color.y = color_data[1];
    color.z = color_data[2];
    color.w = color_data[3];

    material_.GetRoughness() = json["MaterialComponent"]["Roughness"];


}