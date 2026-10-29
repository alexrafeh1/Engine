#include "Motor/common/input/input.h"

std::array<bool, GLFW_KEY_LAST + 1> Input::keys_{false};
std::array<bool, GLFW_KEY_LAST + 1> Input::previousKeys_{};

std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> Input::mouseButtons_{};

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
}


void Input::Update()
{
    previousKeys_ = keys_;
}

void Input::KeyCallback( GLFWwindow* window,  int key, int scancode,int action, int mods)
{
    if (key < 0 || key > GLFW_KEY_LAST)
        return;

    if (action == GLFW_PRESS)
    {
        keys_[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        keys_[key] = false;
    }
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return;

    if (action == GLFW_PRESS)
    {
        mouseButtons_[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        mouseButtons_[button] = false;
    }
}
bool Input::IsKeyDown(Key key)
{
    int glfwKey = static_cast<int>(key);

    if (glfwKey < 0 || glfwKey > GLFW_KEY_LAST)
        return false;

    return keys_[glfwKey];
}

bool Input::IsKeyPressed(Key key)
{
    int glfwKey = static_cast<int>(key);

    if (glfwKey < 0 || glfwKey > GLFW_KEY_LAST)
        return false;

    return keys_[glfwKey] && !previousKeys_[glfwKey];
}

bool Input::IsKeyReleased(Key key)
{
    int glfwKey = static_cast<int>(key);

    if (glfwKey < 0 || glfwKey > GLFW_KEY_LAST)
        return false;

    return !keys_[glfwKey] && previousKeys_[glfwKey];
}


bool Input::IsMouseButtonDown(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;

    return mouseButtons_[button];
}