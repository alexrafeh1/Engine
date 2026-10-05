#include "Motor/common/scene/scene.h"
#include "Motor/common/renderer/forward_renderer.h"
#include "Motor/common/renderer/shadow_renderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>



void Scene::Start(){
	program_manager_.loadProgram("basic", "../data/shaders/basic_vertex.vs", "../data/shaders/basic_fragment.fs");
	program_manager_.loadProgram("shadows", "../data/shaders/shadow_vertex.vs", "../data/shaders/shadow_fragment.fs");

	ecs_.AddComponentType<TransformComponent>("TransformComponent");
	ecs_.AddComponentType<MaterialComponent>("MaterialComponent");
	ecs_.AddComponentType<MeshComponent>("MeshComponent");
	ecs_.AddComponentType<LightComponent>("LightComponent");
	ecs_.AddComponentType<ScriptingComponent>("ScriptingComponent");
	ecs_.AddComponentType<CameraComponent>("CameraComponent");

	render_.Add(std::make_unique<ShadowRenderer>(program_manager_.Get("shadows")));
	render_.Add(std::make_unique<ForwardRendering>(program_manager_.Get("basic")));

	mesh_manager_.Load("../data/assets/car/source/Pony_cartoon.obj");

	printf("%d",scene_serializer_.Load(ecs_, "../data/saves/test.json", mesh_manager_));
}


void Scene::Update(float dt) {
	scripting_system_.Update(ecs_.GetContainer<ScriptingComponent>(), dt);
}


void Scene::Render() {
	RenderContex context{
			.ecs = &ecs_,
	};

	render_.Render(context);

}