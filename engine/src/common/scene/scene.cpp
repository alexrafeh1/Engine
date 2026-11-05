#include "Motor/common/scene/scene.h"
#include "Motor/common/renderer/forward_renderer.h"
#include "Motor/common/renderer/shadow_renderer.h"
#include "Motor/common/renderer/deferred_renderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../deps/glm/glm.hpp"

#include <filesystem>
namespace fs = std::filesystem;

Scene::Scene(GLFWwindow* window)
	: job_system_(std::thread::hardware_concurrency() - 1, window), mesh_manager_(job_system_){
}

void Scene::Start() {

	program_manager_.loadProgram("P_Buffer", "../data/shaders/g_buffer_vertex.vs", "../data/shaders/g_buffer_fragment.fs");
	program_manager_.loadProgram("P_Light", "../data/shaders/deffered_light_vertex.vs", "../data/shaders/deffered_light_fragment.fs");
	program_manager_.loadProgram("P_SSR", "../data/shaders/ssr_vertex.vs", "../data/shaders/ssr_fragment.fs");


	ecs_.AddComponentType<TransformComponent>("TransformComponent");
	ecs_.AddComponentType<MaterialComponent>("MaterialComponent");
	ecs_.AddComponentType<MeshComponent>("MeshComponent");
	ecs_.AddComponentType<LightComponent>("LightComponent");
	ecs_.AddComponentType<ScriptingComponent>("ScriptingComponent");
	ecs_.AddComponentType<CameraComponent>("CameraComponent");

	render_.Add(std::make_unique<DefferedRendering>(program_manager_.Get("P_Buffer"), program_manager_.Get("P_Light"), program_manager_.Get("P_SSR")));


	std::string main_directory = "../data/";

	for (const auto& entry : fs::recursive_directory_iterator(main_directory)) {
		if (entry.is_regular_file() && (entry.path().extension() == ".obj" || entry.path().extension() == ".glb" || entry.path().extension() == ".gltf" || entry.path().extension() == ".fbx")) {
			std::string obj_path = entry.path().generic_string();
			mesh_manager_.LoadAsync(obj_path);
		}
	}

	scene_serializer_.Load(ecs_, "../data/saves/test.json", mesh_manager_);
}


void Scene::Update(float dt) {
	scripting_system_.Update(ecs_.GetContainer<ScriptingComponent>(), dt);

	auto mesh_cont = ecs_.GetContainer<MeshComponent>();

	for (auto mesh_it = mesh_cont.begin(); mesh_it != mesh_cont.end(); ++mesh_it) {
		if (!*mesh_it)continue;
		MeshComponent* m = *mesh_it;
		m->Update();
	}

	if (Input::IsKeyDown(Key::F4)) {
		scene_serializer_.Save(ecs_, "../data/saves/test.json");
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
}


void Scene::Render() {
	RenderContex context{
			.ecs = &ecs_,
			.cam = camera
	};

	render_.Render(context);

}