#include "Motor/common/components/light_component.h"
#include "Motor/common/manager/ecs_manager.h"


void LightComponent::Serialize(nlohmann::json& json) {
    json["LightComponent"] = "";
}
void LightComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {

}