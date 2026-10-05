#ifndef __TRANSFORM_COMPONENT_H__ 
#define __TRANSFORM_COMPONENT_H__ 1

#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/constants.hpp"
#include <vector>
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

static constexpr size_t INVALID_ENTITY = std::numeric_limits<size_t>::max();

class ECSManager;

class TransformComponent {
public:
	TransformComponent();
	~TransformComponent();

	TransformComponent(const TransformComponent& other);
	TransformComponent& operator=(const TransformComponent& other);

	TransformComponent(TransformComponent&& other);
	TransformComponent& operator=(TransformComponent&& other);

	size_t parent = INVALID_ENTITY;
	std::vector<size_t> children;

	glm::mat4 GetLocalMatrix() const;
	glm::mat4 GetWorldMatrix(ECSManager& ecs);

	glm::vec3& GetScale() { return scale_; }
	glm::vec3& GetPosition() { return position_; }
	glm::vec3& GetRotation() { return rotation_; }

	void SetScale(const glm::vec3& scale) { scale_ = scale; } 
	void SetPosition(const glm::vec3& position) { position_ = position; }
	void SetRotation(const glm::vec3& rotation) { rotation_ = rotation; }

	void BindLua(sol::state& lua);

	void Serialize(nlohmann::json& json);
	void Deserialize(const nlohmann::json& json,struct DeserializeContext& context);

private:
	glm::vec3 position_;
	glm::vec3 rotation_;
	glm::vec3 scale_;

};

#endif // !__TRANSFORM_COMPONENT_H__ 
