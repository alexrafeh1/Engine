#include "Motor/common/editor/editor.h"
#include "Motor/common/scene/scene.h"
#include "Motor/common/dev/window.h"

void Editor::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 450");

    panels.push_back(std::make_unique<HierarchyPanel>());
    panels.push_back(std::make_unique<ViewportPanel>());
    panels.push_back(std::make_unique<ComponentsPanel>());


}


void Editor::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}

void Editor::Update(Framebuffer& framebuffer,Scene& scene,Window& window)
{
    ImGui::DockSpaceOverViewport( 0, ImGui::GetMainViewport()  );

    EditorContext ctx{
        .scene = scene,
        .viewport_framebuffer = framebuffer,
        .window = window,
        .selected_entity = selected_entity_
    };

    for (auto& panel : panels) {
        panel->DrawPanel(ctx);
    }
  
}

void Editor::Render()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Editor::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}