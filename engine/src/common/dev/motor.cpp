#include <Motor/common/dev/motor.h>
#include <Motor/common/dev/window.h>
#include <optional>
#include <../deps/imgui/backends/imgui_impl_glfw.h>
#include <../deps/imgui/backends/imgui_impl_opengl3.h>

Motor::~Motor()
{

}

Motor::Motor(Window& window) : window_{ std::move(window) }
{
}


Motor::Motor(Motor&& right) : window_{ std::move(right.window_) } 
{
}

Motor& Motor::operator=(Motor&& right)
{
	window_ = std::move(right.window_);
	return *this;
}



std::optional<Motor> Motor::create(int width, int height)
{
	std::optional<Window> w = Window::createWindow(width, height);
	if (!w)return std::nullopt;
	Window i = std::move(w.value());
	Motor m(i);
	return std::make_optional(std::move(m));
}
