#include "Motor/common/components/transform_component.h"
#include "Motor/common/manager/ecs_manager.h"

TransformComponent::TransformComponent()
{
    position_ = { 0.0f,0.0f,0.0f };
    rotation_ = { 0.0f,0.0f,0.0f };
    scale_ = { 1.0f,1.0f,1.0f };
}

TransformComponent::~TransformComponent()
{
}

TransformComponent::TransformComponent(const TransformComponent& other) :
    position_(other.position_),
    rotation_(other.rotation_),
    scale_(other.scale_)
{
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
    if (this != &other) {
        position_ = other.position_;
        rotation_ = other.rotation_;
        scale_ = other.scale_;
    }
    return *this;
}

TransformComponent::TransformComponent(TransformComponent&& other) : 
    position_(std::move(other.position_)),
    rotation_(std::move(other.rotation_)),
    scale_(std::move(other.scale_))
{
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other)
{
    if (this != &other) {
        position_ = std::move(other.position_);
        rotation_ = std::move(other.rotation_);
        scale_ = std::move(other.scale_);
    }
    return *this;
}

glm::mat4 TransformComponent::GetLocalMatrix() const
{
    glm::mat4 matrix(1.0f);

    matrix = glm::translate(matrix, position_);

    matrix = glm::rotate( matrix,  rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate( matrix, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f) );
    matrix = glm::rotate(matrix, rotation_.z,  glm::vec3(0.0f, 0.0f, 1.0f));

    matrix = glm::scale(matrix, scale_);

    return matrix;
}

glm::mat4 TransformComponent::GetWorldMatrix(ECSManager& ecs) 
{
    glm::mat4 local = GetLocalMatrix();

    if (parent == INVALID_ENTITY)
        return local;

    TransformComponent* parentTransform = ecs.GetComponent<TransformComponent>(parent);

    if (!parentTransform)
        return local;

    return parentTransform->GetWorldMatrix(ecs) * local;
}

void TransformComponent::BindLua(sol::state& lua)
{
    lua["transform"] = this;

    lua.new_usertype<glm::vec3>(
        "Vec3",
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    lua.new_usertype<TransformComponent>(
        "TransformComponent",

        "position", sol::property([](TransformComponent& transform) -> glm::vec3& {
            return transform.GetPosition();
            }
        ),

        "rotation", sol::property([](TransformComponent& transform) -> glm::vec3& {
            return transform.GetRotation();
            }
        ),

        "scale", sol::property([](TransformComponent& transform) -> glm::vec3& {
            return transform.GetScale();
            }
        )
    );

}


void TransformComponent::Serialize(nlohmann::json& json) {
    
    json["TransformComponent"]["position"] = {
        position_.x,
        position_.y,
        position_.z
    };

    json["TransformComponent"]["rotation"] = {
        rotation_.x,
        rotation_.y,
        rotation_.z
    };

    json["TransformComponent"]["scale"] = {
        scale_.x,
        scale_.y,
        scale_.z
    };
}

void TransformComponent::Deserialize(const nlohmann::json& json, DeserializeContext& context)
{
    const auto& data = json["TransformComponent"];

    const auto& position = data["position"];
    const auto& rotation = data["rotation"];
    const auto& scale = data["scale"];

    position_.x = position[0];
    position_.y = position[1];
    position_.z = position[2];

    rotation_.x = rotation[0];
    rotation_.y = rotation[1];
    rotation_.z = rotation[2];

    scale_.x = scale[0];
    scale_.y = scale[1];
    scale_.z = scale[2];
}