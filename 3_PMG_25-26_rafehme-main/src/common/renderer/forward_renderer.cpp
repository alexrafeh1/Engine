#include "Motor/common/renderer/forward_renderer.h"
#include "Motor/common/components/material_component.h"
#include "Motor/common/components/light_component.h"
#include "Motor/common/components/mesh_component.h"
#include "Motor/common/components/transform_component.h"
#include "Motor/common/component_data/mesh_data.h"
#include "Motor/common/components/camera_component.h"


ForwardRendering::ForwardRendering(std::shared_ptr<Program> program):
    lights_program_(program)
{

}

void ForwardRendering::Render(RenderContex& render)
{
    UploadLights(render);
    RenderGeometry(render);
}

void ForwardRendering::UploadLights(RenderContex& render)
{
    std::vector<GPULight> lights;

    for (size_t entity = 0; entity < render.ecs->GetSize(); ++entity)
    {
        auto* light = render.ecs->GetComponent<LightComponent>(entity);

        auto* transform = render.ecs->GetComponent<TransformComponent>(entity);

        if (!light || !transform)
            continue;

        glm::mat4 world =  transform->GetWorldMatrix(*render.ecs);
        glm::vec3 position = glm::vec3(world[3]);

        GPULight gpu{};

        gpu.position = glm::vec4(position, light->radius);
        gpu.color = glm::vec4(light->color,light->intensity);
        gpu.direction = glm::vec4(glm::normalize(light->direction),  static_cast<float>( static_cast<int>(light->type)  ) );

        gpu.parameters = glm::vec4(
                light->constant,
                light->linear,
                light->quadratic,
                light->outerCutOff
            );

        lights.push_back(gpu);
    }

    lightBuffer_.Upload(lights);
    lightBuffer_.Bind(0);
}

void ForwardRendering::RenderGeometry(RenderContex& render)
{
    auto transforms = render.ecs->GetContainer<TransformComponent>();
    auto materials = render.ecs->GetContainer<MaterialComponent>();
    auto meshes = render.ecs->GetContainer<MeshComponent>();

    auto transformIt = transforms.begin();
    auto materialIt = materials.begin();
    auto meshIt = meshes.begin();

    CameraComponent* cam = render.ecs->GetComponent<CameraComponent>(0);
    TransformComponent* tr = render.ecs->GetComponent<TransformComponent>(0);

    glm::mat4 camera_world = tr->GetWorldMatrix(*render.ecs);

    glEnable(GL_DEPTH_TEST);
    for (transformIt, materialIt, meshIt; transformIt != transforms.end() && materialIt != materials.end() && meshIt != meshes.end(); ++transformIt, ++materialIt, ++meshIt) {
        if (!*transformIt || !*materialIt || !*meshIt)
            continue;

        TransformComponent* transform = *transformIt;
        MaterialComponent* material = *materialIt;
        MeshComponent* mesh = *meshIt;

        if (!mesh->HasModel())return;

        lights_program_->UseProgram();

        constexpr int SHADOW_TEXTURE_SLOT = 1;

        for (size_t i = 0; i < render.shadow_maps.size(); ++i)
        {
            const ShadowMap& shadow = render.shadow_maps[i];

            glBindTextureUnit(SHADOW_TEXTURE_SLOT + static_cast<int>(i), shadow.framebuffer.GetDepthTexture());

            std::string shadowSampler = "uShadowMaps[" + std::to_string(i) + "]";
            std::string shadowMatrix = "uLightSpaceMatrices[" + std::to_string(i) + "]";

            lights_program_->SetUniform(shadowSampler.c_str(), SHADOW_TEXTURE_SLOT + static_cast<int>(i));
            lights_program_->SetUniform(shadowMatrix.c_str(), shadow.light_space_matrix);

        }

        lights_program_->SetUniform("uShadowMapCount", static_cast<int>(render.shadow_maps.size()));
        lights_program_->SetUniform("uModel", transform->GetWorldMatrix(*render.ecs));
        lights_program_->SetUniform("uView", glm::inverse(camera_world));
        lights_program_->SetUniform("uProjection", cam->GetProjectionMatrix());
        lights_program_->SetUniform("uCameraPosition", glm::vec3(camera_world[3]));
        lights_program_->SetUniform("uLightCount", static_cast<int>(lightBuffer_.GetCount()));


        const auto& textures = mesh->GetTextures();

        for (const auto& meshData : mesh->GetMeshes()){
            uint32_t textureIndex = meshData->GetTextureIndex();

            if (textureIndex >= textures.size())
                continue;

            const Texture& texture = textures[textureIndex];

            texture.Bind(0);
            lights_program_->SetUniform("uTexture", 0);

            glBindVertexArray(
                meshData->GetVao()
            );

            glDrawElements(
                GL_TRIANGLES,
                meshData->GetCount(),
                GL_UNSIGNED_INT,
                nullptr
            );
        }
    }

    }

