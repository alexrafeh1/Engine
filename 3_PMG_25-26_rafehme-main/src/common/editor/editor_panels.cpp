#include "Motor/common/editor/editor_panels.h"
#include "Motor/common/scene/scene.h"
#include "Motor/common/dev/frame_buffer.h"
#include "Motor/common/dev/window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

void ViewportPanel::DrawPanel(EditorContext& editor_ctx)
{
    ImGui::Begin("Viewport");

    Framebuffer& framebuffer = editor_ctx.viewport_framebuffer;

    ImVec2 size = ImGui::GetContentRegionAvail();

    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);

    if (width > 0 && height > 0)
    {
        framebuffer.Resize(width, height);

        ImGui::Image(static_cast<ImTextureID>(framebuffer.GetColorTexture()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    }

    ImGui::End();
}

void HierarchyPanel::DrawPanel(EditorContext& editor_ctx)
{
    ECSManager& ecs = editor_ctx.scene.GetECS();
    ImGui::Begin("Hierarchy");
    if (ImGui::Button("+ Entity"))
    {
        size_t entity = ecs.AddEntity();
        editor_ctx.selected_entity = entity;
    }
    ImGui::Separator();
    for (size_t i = 0; i < ecs.GetSize(); ++i)
    {
        std::string name = "Entity " + std::to_string(i);
        bool selected = editor_ctx.selected_entity == i;
        if (ImGui::Selectable(name.c_str(), selected))
        {
            editor_ctx.selected_entity = i;
        }
    }
    ImGui::End();
}

void ComponentsPanel::DrawPanel(EditorContext& editor_ctx)
{
    ECSManager& ecs = editor_ctx.scene.GetECS();
    ImGui::Begin("Inspector");
    size_t entity = editor_ctx.selected_entity;
    if (entity == INVALID_ENTITY)
    {
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }
    ImGui::Text("Entity %zu", entity);
    ImGui::Separator();
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponentPopup");
    }
    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        if (!ecs.HasComponent<TransformComponent>(entity)) { if (ImGui::MenuItem("Transform")) { ecs.AddComponent<TransformComponent>(entity); } }
        if (!ecs.HasComponent<MaterialComponent>(entity)) { if (ImGui::MenuItem("Material")) { ecs.AddComponent<MaterialComponent>(entity); } }
        if (!ecs.HasComponent<MeshComponent>(entity)) { if (ImGui::MenuItem("Mesh")) { ecs.AddComponent<MeshComponent>(entity); } }
        if (!ecs.HasComponent<ScriptingComponent>(entity)) { if (ImGui::MenuItem("Script")) { ecs.AddComponent<ScriptingComponent>(entity); } }
        if (!ecs.HasComponent<CameraComponent>(entity)) { if (ImGui::MenuItem("Camera")) { ecs.AddComponent<CameraComponent>(entity); } }
        if (!ecs.HasComponent<LightComponent>(entity)) { if (ImGui::MenuItem("Light")) { ecs.AddComponent<LightComponent>(entity); } }

        ImGui::EndPopup();
    }
    ImGui::Separator();
    if (ecs.HasComponent<TransformComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            TransformComponent* transform = ecs.GetComponent<TransformComponent>(entity);
            glm::vec3 position = transform->GetPosition();
            glm::vec3 rotation = transform->GetRotation();
            glm::vec3 scale = transform->GetScale();
            if (ImGui::DragFloat3("Position", &position.x, 0.1f)) transform->SetPosition(position);
            if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) transform->SetRotation(rotation);
            if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) transform->SetScale(scale);
        }
    }
    if (ecs.HasComponent<MaterialComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            MaterialComponent* material = ecs.GetComponent<MaterialComponent>(entity);
            MaterialData& data = material->GetMaterial();
            ImGui::ColorEdit4("Color", &data.GetColor().x);
            ImGui::DragFloat("Roughness", &data.GetRoughness(), 0.01f, 0.0f, 1.0f);
        }
    }
    if (ecs.HasComponent<CameraComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Camera"))
        {
            CameraComponent* camera = ecs.GetComponent<CameraComponent>(entity);
            ImGui::DragFloat("FOV", &camera->fov_, 0.1f, 1.0f, 179.0f);
            ImGui::DragFloat("Near Plane", &camera->near_plane_, 0.01f, 0.001f, 100.0f);
            ImGui::DragFloat("Far Plane", &camera->far_plane_, 1.0f, 1.0f, 10000.0f);
        }
    }
    if (ecs.HasComponent<MeshComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            MeshComponent* mesh = ecs.GetComponent<MeshComponent>(entity);
            MeshManager& mesh_manager = editor_ctx.scene.GetMeshManager();

            std::string current;
                if (mesh->HasModel()) {
                    const ModelAsset* model = mesh->GetModel();
                    current = model->GetPath().c_str();
                }
                else {
                    current = "None";
                }

                if (ImGui::BeginCombo("Model", current.c_str())) {
                    for (const auto& [path, asset] : mesh_manager.GetModels()) {
                        bool selected = current == path;
                        if (!selected) {
                            if (ImGui::Selectable(path.c_str(), selected)) {
                                mesh->SetModel(mesh_manager.Load(path.c_str()));
                            }
                        }
                    }
                    ImGui::EndCombo();
                }    

        }
    }

    if (ecs.HasComponent<ScriptingComponent>(entity)){
        if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ScriptComponent* script = ecs.GetComponent<ScriptComponent>(entity);
            std::string script_path = script->GetScriptPath();

            char buffer[256];
            std::strncpy(buffer, script_path.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';

            if (ImGui::InputText("Script", buffer, sizeof(buffer)))
            {
                script->SetScriptPath(buffer);
            }

            if (ImGui::Button("Reload Script"))
            {
                script->Reload();
            }
        }
    }
    ImGui::End();
}