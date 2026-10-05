#include "Motor/common/scene/scene_serializer.h"
#include "Motor/common/scene/scene.h"
#include "Motor/common/manager/ecs_manager.h"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;


bool SceneSerializer::Save(const ECSManager& ecs, const std::string& path)
{
    json root;

    root["entities"] = json::array();


    for (size_t entity = 0; entity < ecs.GetSize(); ++entity)
    {
        json entityJson;

        entityJson["id"] = entity;

        ecs.SerializeComponents(entityJson, entity);

        root["entities"].push_back(entityJson);
    }

    std::ofstream file(path);

    if (!file.is_open())
        return false;

    file << root.dump(4);

    return true;
}

bool SceneSerializer::Load(ECSManager& ecs, const std::string& path, MeshManager& mesh_manager)
{
    std::ifstream file(path);

    if (!file.is_open())
        return false;

    json root;

    try
    {
        file >> root;
    }
    catch (const json::parse_error&)
    {
        return false;
    }


    if (!root.contains("entities"))
        return false;

    DeserializeContext context{
        .meshManager = mesh_manager,
        .ecs = ecs
    };

    for (const auto& entityJson : root["entities"])
    {
        size_t entity = ecs.AddEntity();

        ecs.DeserializeComponents(entityJson, entity, context);
    }

    return true;
}
