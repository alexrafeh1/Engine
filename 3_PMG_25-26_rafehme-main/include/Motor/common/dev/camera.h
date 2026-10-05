#ifndef __CAMERA_H__
#define __CAMERA_H__ 1

#include <../deps/glm/glm.hpp>
#include <../deps/glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;

    glm::vec3 GetPosition() {
        return position_;
    }

    void Update(float deltaTime);

    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);

    void Rotate(float yaw, float pitch);

private:
    glm::vec3 position_{ 0.0f, 0.0f, 3.0f };

    float yaw_ = -90.0f;
    float pitch_ = 0.0f;

    float fov_ = 60.0f;
    float aspect_ = 16.0f / 9.0f;
    float nearPlane_ = 0.1f;
    float farPlane_ = 1000.0f;

    float movementSpeed_ = 5.0f;
};

#endif // !__CAMERA_H__
