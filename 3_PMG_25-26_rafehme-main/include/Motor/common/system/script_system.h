#ifndef __SCRIPT_SYSYTEM_H__ 
#define __SCRIPT_SYSYTEM_H__ 1

#include "Motor/common/manager/ecs_manager.h"

class ScriptingComponent;

class ScriptSystem {
public:

	void Update(ECSManager::Container<ScriptingComponent> script_container,float dt);

private:
};

#endif // !__SCRIPT_SYSYTEM_H__ 
