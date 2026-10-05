#include "Motor/common/renderer/shadow_renderer.h"
#include "Motor/common/components/light_component.h"
#include "Motor/common/components/mesh_component.h"
#include "Motor/common/components/transform_component.h"
#include "Motor/common/component_data/mesh_data.h"

ShadowRenderer::ShadowRenderer(std::shared_ptr<Program> shadow_program):
    shadow_program_(std::move(shadow_program))
{
}







void ShadowRenderer::Render(RenderContex& context)
{
    context.shadow_maps.clear();
    glEnable(GL_DEPTH_TEST);

    auto light_container = context.ecs->GetContainer<LightComponent>();
    size_t shadowIndex = 0;

    for (auto light_it = light_container.begin(); light_it != light_container.end(); ++light_it) {
        if (!*light_it)continue;
        LightComponent* light = *light_it;

        glm::mat4 lightSpaceMatrix;

        if (light->type == LightType::Directional_Light) {

            glm::vec3 lightDirection = glm::normalize(light->direction);
            glm::vec3 lightPosition = -lightDirection * 20.0f;
            glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 1000.0f);
            lightSpaceMatrix = lightProjection * lightView;
        }

        if (light->type == LightType::Point_Light) {

        }

        if (light->type == LightType::Spot_Light) {

        }

        if (shadowIndex >= shadow_maps_.size())
        {
            CreateShadowMap();
        }
        ShadowMap& shadowMap = shadow_maps_[shadowIndex];

        shadowMap.framebuffer.Bind();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadow_program_->UseProgram();
        shadow_program_->SetUniform("uLightSpaceMatrix", lightSpaceMatrix);

        //RenderShadowCasters(context);
        shadowMap.framebuffer.Unbind();
        
        ++shadowIndex;
        
        context.shadow_maps.push_back(std::move(shadowMap));
    }
 
}


void ShadowRenderer::RenderShadowCasters(
    RenderContex& render)
{
    auto transforms = render.ecs->GetContainer<TransformComponent>();
    auto meshes = render.ecs->GetContainer<MeshComponent>();

    auto transformIt = transforms.begin();
    auto meshIt = meshes.begin();

    while (transformIt != transforms.end() && meshIt != meshes.end())
    {
        TransformComponent* transform = *transformIt;
        MeshComponent* mesh = *meshIt;

        ++transformIt;
        ++meshIt;

        if (!transform || !mesh)
            continue;

        shadow_program_->SetUniform("uModel", transform->GetWorldMatrix(*render.ecs) );

        for (const auto& meshData : mesh->GetMeshes())
        {
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

void ShadowRenderer::CreateShadowMap()
{
    ShadowMap shadowMap;

    shadowMap.framebuffer.Create(
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
        FramebufferTextureType::Depth
    );

    if (!shadowMap.framebuffer.IsComplete())
    {
        std::cout << "Shadow framebuffer incompleto\n";
        return;
    }

    shadow_maps_.push_back(std::move(shadowMap) );
}
