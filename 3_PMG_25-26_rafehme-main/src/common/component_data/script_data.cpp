#include "Motor/common/component_data/script_data.h"
#include "../deps/glm/glm.hpp"
#include "Motor/common/components/transform_component.h"
#include "Motor/common/input/input.h"


ScriptData::ScriptData() {
    lua_.open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::table
    );

    lua_.new_enum<Key>(
        "Key",
        {
            {"W", Key::W},
            {"A", Key::A},
            {"S", Key::S},
            {"D", Key::D},
            {"Space", Key::Space},
            {"Escape", Key::Escape},
            {"LeftShift", Key::LeftShift}
        }
    );

    auto input = lua_.create_table();

    input["is_key_down"] =
        [](Key key)
        {
            return Input::IsKeyDown(key);
        };

    input["is_key_pressed"] =
        [](Key key)
        {
            return Input::IsKeyPressed(key);
        };

    input["is_key_released"] =
        [](Key key)
        {
            return Input::IsKeyReleased(key);
        };

    lua_["input"] = input;
}

bool ScriptData::LoadScript(const std::string& path, ECSManager& ecs, size_t entity)
{
    ecs.BindComponentsToLua(lua_, entity);

    sol::load_result script = lua_.load_file(path);

    if (!script.valid())
    {
        sol::error error = script;

        std::cerr
            << "Lua load error: "
            << error.what()
            << '\n';

        return false;
    }

    sol::protected_function_result result = script();

    if (!result.valid())
    {
        sol::error error = result;

        std::cerr
            << "Lua execution error: "
            << error.what()
            << '\n';

        return false;
    }

    sol::protected_function start = lua_["start"];

    if (start.valid()) {
        start();
    }
    path_ = path;
    entiny_ = entity;
    return true;
}



void ScriptData::Update(float deltaTime) {
    sol::protected_function update = lua_["update"];

    if (!update.valid())
        return;

    sol::protected_function_result result = update(deltaTime);

    if (!result.valid())
    {
        sol::error error = result;

        std::cerr
            << "Lua update error: "
            << error.what()
            << '\n';
    }
}