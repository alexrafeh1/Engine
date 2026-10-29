#ifndef __CAMERA_H__
#define __CAMERA_H__ 1

#include <../deps/glm/glm.hpp>
#include <../deps/glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

class Camera{
public:
    glm::vec3 position{ 0.0f, 0.0f, 5.0f };

    float yaw = -90.0f;
    float pitch = 0.0f;

    float movementSpeed = 5.0f;
    float mouseSensitivity = 0.1f;

    glm::vec3 GetForward() const
    {
        const float y = glm::radians(yaw);
        const float p = glm::radians(pitch);

        return glm::normalize(glm::vec3(
            std::cos(y) * std::cos(p),
            std::sin(p),
            std::sin(y) * std::cos(p)
        ));
    }

    glm::vec3 GetRight() const
    {
        return glm::normalize(
            glm::cross(GetForward(), glm::vec3(0, 1, 0))
        );
    }

    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(
            position,
            position + GetForward(),
            glm::vec3(0, 1, 0)
        );
    }

    glm::mat4 GetProjectionMatrix(float aspectRatio) const
    {
        return glm::perspective(
            glm::radians(60.0f), 
            aspectRatio,         
            0.1f,                
            1000.0f              
        );
    }

    void MoveForward(float amount)
    {
        position += GetForward() * amount;
    }

    void MoveRight(float amount)
    {
        position += GetRight() * amount;
    }

    void MoveVertical(float amount)
    {
        position.y += amount;
    }

    void Rotate(float deltaX, float deltaY)
    {
        yaw += deltaX * mouseSensitivity;
        pitch -= deltaY * mouseSensitivity;

        pitch = std::clamp(pitch, -89.0f, 89.0f);
    }
};

#endif // !__CAMERA_H__
