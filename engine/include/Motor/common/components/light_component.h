#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__ 1

#include <vector>
#include "../deps/glm/glm.hpp"
#include <glad/gl.h>
#include <nlohmann/json.hpp>



struct GPULight
{
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 direction;
    glm::vec4 parameters;
};

class LightBuffer
{
public:
    LightBuffer();
    ~LightBuffer();

    void Upload(const std::vector<GPULight>& lights);
    void Bind(GLuint binding) const;

    GLuint GetCount() {
        return count_;
    };

private:
    GLuint buffer_ = 0;
    GLuint count_ = 0;
};

enum class LightType
{
    Directional_Light = 0,
    Spot_Light,
    Point_Light
};


struct LightComponent
{
    LightType type = LightType::Directional_Light;

    glm::vec3 color{ 1.0f };
    float intensity = 1.0f;

    glm::vec3 direction{ 0.0f, 0.0f, -1.0f };

    float radius = 10.0f;

    float constant = 1.0f;
    float linear = 0.007f;
    float quadratic = 0.0002f;

    float cutOff =
        glm::cos(glm::radians(12.5f));

    float outerCutOff =
        glm::cos(glm::radians(17.5f));

    void SetDirection(const glm::vec3& dir)
    {
        if (glm::length(dir) > 0.0f)
            direction = glm::normalize(dir);
    }

    void SetColor(const glm::vec3& col)
    {
        color = col;
    }

    void Serialize(nlohmann::json& json);
    void Deserialize(const nlohmann::json& json, struct DeserializeContext& context);


};
#endif // !__LIGHT_COMPONENT_H__
