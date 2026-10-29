#ifndef __WINDEO_H__
#define __WINDEO_H__ 1

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <memory>
#include <map>


class Window {
public:
	~Window();

	
	Window(const Window& right) = delete;
	Window& operator=(const Window& right) = delete;

	Window(Window&& right);
	Window& operator=(Window&& right);

	static std::optional<Window> createWindow(int width, int height);
	GLFWwindow* GetWindow();
	
	double GetTimeD() {
		return glfwGetTime();
	}

	float GetTimeF() {
		return (float)glfwGetTime();
	}

	double GetDtD() {
		return dt;
	}

	float GetDtF() {
		return (float)dt;
	}

	void SetUpOpenGl();
	void StartFrame();
	void EndFrame();
	bool isClosed();

	int height_, width_;

private:
	Window(GLFWwindow* gw, int height, int width);
	GLFWwindow* window_;
	double last_time, current_time, dt;
};

#endif // !__WINDEO_H_
