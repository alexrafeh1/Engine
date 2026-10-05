#include "Motor/common/system/script_system.h"
#include "Motor/common/components/scripting_component.h"


void ScriptSystem::Update(ECSManager::Container<ScriptingComponent> script_container,float dt)
{

	for (ECSManager::Iterator script_it = script_container.begin(); script_it != script_container.end(); ++script_it) {
		if (!*script_it)continue;
		ScriptingComponent* script = *script_it;
		if (!script->GetScript())continue;
		ScriptData* data = script->GetScript();
		data->Update(dt);
	}

}
