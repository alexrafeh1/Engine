#include "Motor/common/dev/window.h"
#include <../deps/imgui/backends/imgui_impl_glfw.h>
#include <../deps/imgui/backends/imgui_impl_opengl3.h>

static std::map<GLFWwindow*, Window*> window_map_;


Window::~Window()
{

	if (window_) {
		window_map_.erase(window_);
		glfwDestroyWindow(window_);
		window_ = nullptr;
	}
}

Window::Window(GLFWwindow* gw, int width, int height) :
	window_{ gw } , 
	height_{ height } , 
	width_{ width }
{
	window_map_[gw] = this;
	current_time = 0.0f;
	last_time = 0.0f;
	dt = 0.0f;
}

Window::Window(Window&& right){
	window_ = right.window_;
	if (window_) {
		window_map_[window_] = this;
	}
	height_ = right.height_;
	width_ = right.width_;
	current_time = right.current_time;
	last_time = right.last_time;
	dt = right.dt;
	right.window_ = nullptr;
}

Window& Window::operator=(Window&& other){
	if (this != &other) {
		if (window_) glfwDestroyWindow(window_);
		window_ = other.window_;
		if (window_) {
			window_map_[window_] = this;
		}
		height_ = other.height_;
		width_ = other.width_;
		current_time = other.current_time;
		last_time = other.last_time;
		dt = other.dt;
		other.window_ = nullptr;
	}
	return *this;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

	auto it_w = window_map_.find(window);
	if (it_w == window_map_.end())return;

    if (width == 0 || height == 0) return;
	
	Window* win = it_w->second;
    win->width_  = width;
    win->height_ = height;
}

std::optional<Window> Window::createWindow(int width, int height) 
{ 
	if (!glfwInit())
	{ 
		return std::nullopt; 
	} 
	//Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	GLFWwindow* gw = glfwCreateWindow(width, height, "Engine", NULL, NULL); 
	
	if (gw == nullptr) { 
		return std::nullopt; 
	} 
	glfwMakeContextCurrent(gw); 

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		return std::nullopt;
	}

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(gw, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);


	glfwSetFramebufferSizeCallback(gw, framebuffer_size_callback);

	glfwSwapInterval(0);
	
	return std::make_optional(Window{ gw, fbWidth, fbHeight });

}



GLFWwindow* Window::GetWindow() {
	return window_;
}

void Window::SetUpOpenGl()
{
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT_AND_BACK);
	glDepthFunc(GL_LESS);
}
void Window::StartFrame()
{
	glfwPollEvents();

	current_time = GetTimeD();
	dt = current_time - last_time;
	last_time = current_time;
}



void Window::EndFrame()
{
	glfwSwapBuffers(window_);
	glfwPollEvents();
}



bool Window::isClosed()
{
	return glfwWindowShouldClose(window_);
}
