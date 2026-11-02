#include "Motor/common/renderer/deferred_renderer.h"
#include "Motor/common/renderer/forward_renderer.h"
#include "Motor/common/components/material_component.h"
#include "Motor/common/components/light_component.h"
#include "Motor/common/components/mesh_component.h"
#include "Motor/common/components/transform_component.h"
#include "Motor/common/component_data/mesh_data.h"
#include "Motor/common/components/camera_component.h"

float vertices[] = {
    // Posición       // UV
    -1.0f, -1.0f,     0.0f, 0.0f,
     1.0f, -1.0f,     1.0f, 0.0f,
     1.0f,  1.0f,     1.0f, 1.0f,
    -1.0f,  1.0f,     0.0f, 1.0f
};
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

DefferedRendering::DefferedRendering(std::shared_ptr<Program> buffer_program, std::shared_ptr<Program> light_program):
    G_Buffer_Program(buffer_program), Light_Program(light_program)
{
    G_Buffer = std::make_unique<Framebuffer>();
    G_Buffer->Create(1024, 1024, FramebufferTextureType::GBuffer);

    Light_Buffer = std::make_unique<Framebuffer>();
    Light_Buffer->Create(1024, 1024, FramebufferTextureType::Lights);

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 4 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 1, 0);
}

void DefferedRendering::Render(RenderContex& render)
{
    G_Buffer->Bind();
    RenderGBuffer(render);
    G_Buffer->Unbind();

    RenderLights(render);
}

void DefferedRendering::RenderGBuffer(RenderContex& render)
{
    auto transforms = render.ecs->GetContainer<TransformComponent>();
    auto materials = render.ecs->GetContainer<MaterialComponent>();
    auto meshes = render.ecs->GetContainer<MeshComponent>();

    auto transformIt = transforms.begin();
    auto materialIt = materials.begin();
    auto meshIt = meshes.begin();

    G_Buffer_Program->UseProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glViewport(0, 0, 1024, 1024);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    for (transformIt, materialIt, meshIt; transformIt != transforms.end() && materialIt != materials.end() && meshIt != meshes.end(); ++transformIt, ++materialIt, ++meshIt) {
        if (!*transformIt || !*materialIt || !*meshIt)
            continue;

        TransformComponent* transform = *transformIt;
        MaterialComponent* material = *materialIt;
        MeshComponent* mesh = *meshIt;

        if (!mesh->HasModel())continue;

        G_Buffer_Program->SetUniform("uModel", transform->GetWorldMatrix(*render.ecs));
        G_Buffer_Program->SetUniform("uView", render.cam.GetViewMatrix());
        G_Buffer_Program->SetUniform("uProjection", render.cam.GetProjectionMatrix(1.0f));
        G_Buffer_Program->SetUniform("uColor", material->GetColor());
        G_Buffer_Program->SetUniform("uRoughness", material->GetRoughness());
        G_Buffer_Program->SetUniform("uMetallic", material->GetMetallic());
        G_Buffer_Program->SetUniform("uAO", material->GetAO());


        bool albedo = false;
        bool normal = false;
        bool roughness = false;
        bool metallic = false;
        bool ao = false;

        for (const auto& meshData : mesh->GetMeshes()) {
            MaterialTextures& textures = meshData->GetTextures();

            if (textures.albedo) {
                textures.albedo.value().Bind(0);
                albedo = true;
            }

            if (textures.normal) {
                textures.normal.value().Bind(1);
                normal = true;
            }

            if (textures.roughness) {
                textures.roughness.value().Bind(2);
                roughness = true;
            }

            if (textures.metallic) {
                textures.metallic.value().Bind(3);
                metallic = true;
            }

            if (textures.ao) {
                textures.ao.value().Bind(4);
                ao = true;
            }

            G_Buffer_Program->SetUniform("uHasAlbedo", albedo);
            G_Buffer_Program->SetUniform("uHasNormal", normal);
            G_Buffer_Program->SetUniform("uHasRoughness", roughness);
            G_Buffer_Program->SetUniform("uHasMetallic", metallic);
            G_Buffer_Program->SetUniform("uHasAO", ao);

            G_Buffer_Program->SetUniform("uDiffuseTexture", 0);
            G_Buffer_Program->SetUniform("uNormalTexture", 1);
            G_Buffer_Program->SetUniform("uRoughnessTexture", 2);
            G_Buffer_Program->SetUniform("uMetallicTexture", 3);
            G_Buffer_Program->SetUniform("uAOTexture", 4);

            glDisable(GL_CULL_FACE);
            glBindVertexArray(meshData->GetVao());
            glDrawElements(GL_TRIANGLES, meshData->GetCount(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }
}

void DefferedRendering::RenderLights(RenderContex& render)
{

    Light_Program->UseProgram();

    glBindTextureUnit(1, G_Buffer->GetAlbedoTexture_());
    glBindTextureUnit(2, G_Buffer->GetNormalTexture());
    glBindTextureUnit(3, G_Buffer->GetMaterialTexture());
    glBindTextureUnit(4, G_Buffer->GetDepthTexture());

    Light_Program->SetUniform("gDiffuse", 1);
    Light_Program->SetUniform("gNormal", 2);
    Light_Program->SetUniform("gMaterial", 3);
    Light_Program->SetUniform("gDepth", 4); 

    Light_Program->SetUniform("UInverseProjection", glm::inverse(render.cam.GetProjectionMatrix(1.0f)));
    Light_Program->SetUniform("uInverseView", glm::inverse(render.cam.GetViewMatrix()));
    Light_Program->SetUniform("uCamPos", render.cam.position);


    glViewport(0, 0, 1024, 1024);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glBindVertexArray(VAO);

    auto lights_container = render.ecs->GetContainer<LightComponent>();
    auto transform_container = render.ecs->GetContainer<TransformComponent>();

    auto light_begin = lights_container.begin();
    auto transform_begin = transform_container.begin();

    bool first = true;
    for (light_begin,transform_begin ; light_begin != lights_container.end() && transform_begin != transform_container.end(); ++light_begin,++transform_begin) {
        if (!*light_begin || !*transform_begin)continue;
        LightComponent* light = *light_begin;
        TransformComponent* transform = *transform_begin;
        SetLightUniforms(light);

        glm::mat4 world = transform->GetWorldMatrix(*render.ecs);
        glm::vec3 position = glm::vec3(world[3]);

        Light_Program->SetUniform("light_pos", position);
        Light_Program->SetUniform("uFirstPass", first);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        first = false;
    }
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void DefferedRendering::SetLightUniforms(LightComponent* light){
    Light_Program->SetUniform("light_dirr", light->direction);
    Light_Program->SetUniform("light_color", light->color);
    Light_Program->SetUniform("light_type", static_cast<int>(light->type));

}
