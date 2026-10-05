#ifndef __INPUT_H__
#define __INPUT_H__ 1


#include <GLFW/glfw3.h>
#include <array>

enum class Key
{
    W = GLFW_KEY_W,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,

    Space = GLFW_KEY_SPACE,
    Escape = GLFW_KEY_ESCAPE,
    LeftShift = GLFW_KEY_LEFT_SHIFT
};

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool IsKeyDown(Key key);
    static bool IsKeyPressed(Key key);
    static bool IsKeyReleased(Key key);

    static bool IsMouseButtonDown(int button);

    static std::array<bool, GLFW_KEY_LAST + 1> keys_;
    static std::array<bool, GLFW_KEY_LAST + 1> previousKeys_;

    static std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons_;
private:
    static void KeyCallback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );

    static void MouseButtonCallback(
        GLFWwindow* window,
        int button,
        int action,
        int mods
    );



};

#endif // !__INPUT_H__
