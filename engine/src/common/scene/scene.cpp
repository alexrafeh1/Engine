#include "Motor/common/scene/scene.h"
#include "Motor/common/renderer/forward_renderer.h"
#include "Motor/common/renderer/shadow_renderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../deps/glm/glm.hpp"



Scene::Scene(GLFWwindow* window)
	: job_system_(std::thread::hardware_concurrency() - 1, window), mesh_manager_(job_system_), pl_sdf({0,0,0}, 400.0f), vc(pl_sdf), chunk_manager(vc, job_system_)
{
}

void Scene::Start() {
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

	printf("Scene loaded: %d\n", static_cast<int>(scene_serializer_.Load(ecs_, "../data/saves/test.json", mesh_manager_)));


	chunk_manager.GenerateChunks({ 0, 0, 0 });
	


	size_t planet = ecs_.AddEntity();

	ecs_.AddComponent<TransformComponent>(planet)
		->GetPosition() = { 0,0,-550.0f};

	ecs_.AddComponent<MaterialComponent>(planet);

	ecs_.AddComponent<MeshComponent>(planet)
		->SetModel(chunk_manager.GetAsset());
	
}


void Scene::Update(float dt) {
	scripting_system_.Update(ecs_.GetContainer<ScriptingComponent>(), dt);

	auto mesh_cont = ecs_.GetContainer<MeshComponent>();

	for (auto mesh_it = mesh_cont.begin(); mesh_it != mesh_cont.end(); ++mesh_it) {
		if (!*mesh_it)continue;
		MeshComponent* m = *mesh_it;
		m->Update();
	}

	if (Input::IsKeyDown(Key::W)) {
		camera.MoveForward(100.0f * dt);
	}

	if (Input::IsKeyDown(Key::S)) {
		camera.MoveForward(-100.0f * dt);
	}

	if (Input::IsKeyDown(Key::A)) {
		camera.MoveRight(-10.0f * dt);
	}

	if (Input::IsKeyDown(Key::D)) {
		camera.MoveRight(10.0f * dt);
	}

	if (Input::IsKeyDown(Key::Q)) {
		camera.MoveVertical(10.0f * dt);
	}

	if (Input::IsKeyDown(Key::E)) {
		camera.MoveVertical(-10.0f * dt);
	}

	static float pos = 400.0f;
	if (Input::IsKeyPressed(Key::Space)) {
		// Excavación en la superficie del planeta
		glm::vec3 excavationCenter(0.0f, 0.0f, pos);
		float excavationRadius = 10.0f;

		pl_sdf.CarveSphere(excavationCenter, excavationRadius);
		chunk_manager.GetAffectedChunks(
			excavationCenter,
			excavationRadius
		);

		pos -= 10;
		printf("sd");
	}

	chunk_manager.Update(camera.position, { 0,0,-550 });

}


void Scene::Render() {
	RenderContex context{
			.ecs = &ecs_,
			.cam = camera
	};

	render_.Render(context);

}