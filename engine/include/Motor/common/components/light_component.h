#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__ 1

#include <vector>
#include "../deps/glm/glm.hpp"
#include <glad/gl.h>
#include <nlohmann/json.hpp>
#include <string>




enum class LightType : int{
    Directional_Light = 0,
    Spot_Light,
    Point_Light
};


struct LightComponent
{
    LightType type_ = LightType::Directional_Light;
    glm::vec3 color{ 1.0f };
    float intensity = 1.0f;
    float inerCutoff = glm::cos(glm::radians(12.5f));
    float outerCutoff = glm::cos(glm::radians(17.5f));
    glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
    float radius = 1000.0f;

    void SetDirection(const glm::vec3& dir)
    {
        if (glm::length(dir) > 0.0f)
            direction = glm::normalize(dir);
    }

    void SetColor(const glm::vec3& col)
    {
        color = col;
    }

    glm::vec3& GetColor() {
        return color;
    }

    LightType& GetType() {
        return type_;
    }

    std::string GetTypeName() {
        switch (type_) {
        case LightType::Directional_Light:
            return std::string("Directional");
            break;

        case LightType::Spot_Light:
            return std::string("Spot");


        case LightType::Point_Light:
            return std::string("Point");
            break;
        }

        return std::string("Empty");
    }

    std::vector<std::string> GetAllTypeNames() {
        std::vector<std::string> names;
        names.push_back(std::string("Directional"));
        names.push_back(std::string("Spot"));
        names.push_back(std::string("Point"));
        return names;
    }

    void SetType(int type) {
        type_ =  static_cast<LightType>(type);
    }

    glm::vec3& GetDirection() {
        return direction;
    }


    void Serialize(nlohmann::json& json);
    void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);


};
#endif // !__LIGHT_COMPONENT_H__
