#ifndef  __MOTOR_H__
#define __MOTOTR_H__ 1

#include <Motor/common/dev/window.h>
#include <optional>


class Motor {
public:
	~Motor();

    Motor(const Motor& right) = delete;
    Motor& operator=(const Motor& right) = delete;

    Motor(Motor&& right);
    Motor& operator=(Motor&& right);



    static std::optional<Motor> create(int width,int height);
    Window& GetWindow() { return window_; }

private:
	Motor(Window& window_);
	Window window_;
};
#endif // ! __MOTOR_H__
