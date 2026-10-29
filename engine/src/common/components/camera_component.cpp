#include "Motor/common/components/camera_component.h"

glm::mat4 CameraComponent::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov_), aspect_, near_plane_, far_plane_);
}

void CameraComponent::BindLua(sol::state& lua) {
    lua["camera"] = this;
}

void CameraComponent::Serialize(nlohmann::json& json) {
   json["CameraComponent"]["Fov"] = fov_;
   json["CameraComponent"]["Aspect"] = aspect_;
   json["CameraComponent"]["Nearplane"] = near_plane_;
   json["CameraComponent"]["FarPLane"] = far_plane_;
}

void CameraComponent::Deserialize(const nlohmann::json& json, struct DeserializeContext& context) {
    fov_ = json["CameraComponent"]["Fov"];
    aspect_ = json["CameraComponent"]["Aspect"];
    near_plane_ = json["CameraComponent"]["Nearplane"];
    far_plane_ = json["CameraComponent"]["FarPLane"];
}
