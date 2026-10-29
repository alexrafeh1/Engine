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
#include "Motor/common/dev/job_system.h"
#include "Motor/common/planet/generate_voxel.h"
#include "Motor/common/planet/voxel_conditioner.h"
#include "Motor/common/input/input.h"
#include "Motor/common/planet/chunks.h"
#include "Motor/common/planet/chunk_manager.h"
#include "Motor/common/dev/camera.h"

class Scene
{
public:
    explicit Scene(GLFWwindow* window);

    void Start();
    void Update(float dt);
    void Render();

    ECSManager& GetECS()
    {
        return ecs_;
    }

    MeshManager& GetMeshManager()
    {
        return mesh_manager_;
    }


private:
    ECSManager ecs_;
    RenderManager render_;
    MeshManager mesh_manager_;
    ProgramManager program_manager_;
    ScriptSystem scripting_system_;
    SceneSerializer scene_serializer_;
    JobSystem job_system_;
    Camera camera;
    ChunkManager chunk_manager;
    PlanetSDF pl_sdf;
    VoxelConditioner vc;

};

#endif // !__SCENE_H__
