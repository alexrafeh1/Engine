#ifndef __EDITOR_H__ 
#define __EDITOR_H__ 1

#include "Motor/common/dev/frame_buffer.h"


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Motor/common/editor/editor_panels.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

class Window;

class Editor {
public:
    void Init(GLFWwindow* window);
    void BeginFrame();
    void Update(class Framebuffer& framebuffer,class Scene& scene, Window& window);
    void Render();
    void Shutdown();

private:
    std::vector<std::unique_ptr<IEditorPanels>> panels;
    size_t selected_entity_ = INVALID_ENTITY;
};

#endif // !__EDITOR_H__ 
