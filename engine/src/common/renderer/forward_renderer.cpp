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
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glClearDepth(1.0);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (transformIt, materialIt, meshIt; transformIt != transforms.end() && materialIt != materials.end() && meshIt != meshes.end(); ++transformIt, ++materialIt, ++meshIt) {
        if (!*transformIt || !*materialIt || !*meshIt)
            continue;

        TransformComponent* transform = *transformIt;
        MaterialComponent* material = *materialIt;
        MeshComponent* mesh = *meshIt;

        if (!mesh->HasModel())continue;

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
        lights_program_->SetUniform("uView", render.cam.GetViewMatrix());
        lights_program_->SetUniform("uProjection", render.cam.GetProjectionMatrix(1.0f));
        lights_program_->SetUniform("uCameraPosition", render.cam.position);
        lights_program_->SetUniform("uColor", material->GetColor());
        lights_program_->SetUniform("uPlanetCenter", glm::vec3(0.0f, 0.0f, -550.0f));
        lights_program_->SetUniform("uPlanetRadius", 400.0f);


    }

    }

