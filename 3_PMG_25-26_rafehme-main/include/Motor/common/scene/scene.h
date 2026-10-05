#ifndef __SCENE_H__
#define __SCENE_H__ 1

#include "Motor/common/manager/ecs_manager.h"
#include "Motor/common/manager/render_manager.h"
#include "Motor/common/manager/mesh_manager.h"
#include "Motor/common/manager/program_manager.h"
#include "Motor/common/scene/scene_serializer.h"
#include "Motor/common/system/script_system.h"
#include "Motor/common/components/camera_component.h"
#include "Motor/common/components/transform_component.h"
#include "Motor/common/components/mesh_component.h"
#include "Motor/common/components/material_component.h"
#include "Motor/common/components/scripting_component.h"
#include "Motor/common/components/light_component.h"


class Scene {
public:
	
	Scene() = default;
	
	void Start();

	void Update(float dt);

	void Render();

	 ECSManager& GetECS()  {
		return ecs_;
	 }

	 MeshManager& GetMeshManager() {
		 return mesh_manager_;
	 }


private:
	ECSManager ecs_;
	RenderManager render_;
	MeshManager mesh_manager_;
	ProgramManager program_manager_;
	ScriptSystem scripting_system_;
	SceneSerializer scene_serializer_;
};

#endif // !__SCENE_H__
