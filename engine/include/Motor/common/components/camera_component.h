#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__ 1


#include <../deps/glm/glm.hpp>
#include <../deps/glm/gtc/matrix_transform.hpp>
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>


class CameraComponent {
public:
    float fov_ = 90.0f;
    float aspect_ = 1.0f;
    float near_plane_ = 0.1f;
    float far_plane_ = 1000.0f;

    glm::mat4 GetProjectionMatrix() const;

    void BindLua(sol::state& lua);

    void Serialize(nlohmann::json& json);
    void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);
};

#endif // !__CAMERA_COMPONENT_H__
