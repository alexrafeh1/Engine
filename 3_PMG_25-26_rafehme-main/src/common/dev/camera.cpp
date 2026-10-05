#include "Motor/common/dev/camera.h"

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov_),aspect_, nearPlane_,farPlane_);
}


glm::mat4 Camera::GetViewMatrix() const {
    glm::vec3 direction;

    direction.x =
        cos(glm::radians(yaw_)) *
        cos(glm::radians(pitch_));

    direction.y =
        sin(glm::radians(pitch_));

    direction.z =
        sin(glm::radians(yaw_)) *
        cos(glm::radians(pitch_));

    direction = glm::normalize(direction);

    return glm::lookAt(
        position_,
        position_ + direction,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}



void Camera::Update(float deltaTime) {

}

void Camera::MoveForward(float amount)
{
    glm::vec3 forward;

    forward.x =
        glm::cos(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward.y =
        glm::sin(glm::radians(pitch_));

    forward.z =
        glm::sin(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward =
        glm::normalize(forward);

    position_ += forward * amount;
}

void Camera::MoveRight(float amount)
{
    glm::vec3 forward;

    forward.x =
        glm::cos(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward.y =
        glm::sin(glm::radians(pitch_));

    forward.z =
        glm::sin(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward =
        glm::normalize(forward);

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    position_ += right * amount;
}

void Camera::MoveUp(float amount)
{
    glm::vec3 forward;

    forward.x =
        glm::cos(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward.y =
        glm::sin(glm::radians(pitch_));

    forward.z =
        glm::sin(glm::radians(yaw_)) *
        glm::cos(glm::radians(pitch_));

    forward =
        glm::normalize(forward);

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f)));

    position_ += right * amount;
}

void Camera::Rotate(float yaw, float pitch)
{
    yaw_ += yaw;
    pitch_ += pitch;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;

    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    if (yaw_ > 360.0f)
        yaw_ -= 360.0f;

    if (yaw_ < -360.0f)
        yaw_ += 360.0f;
}