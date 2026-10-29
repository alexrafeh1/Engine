#ifndef __SCRIPT_DATA_H__
#define __SCRIPT_DATA_H__ 1

#include <sol/sol.hpp>
#include <string>
#include "Motor/common/manager/ecs_manager.h"

class ScriptData {

public:
	ScriptData();

	bool LoadScript(const std::string& path,const ECSManager& ecs,size_t entity);

	void Update(float deltaTime);

	std::string& GetPath() {
		return path_;
	}

	void SetPath(char* path) {
		path_ = path;
	}

	const size_t& GetEntity() const {
		return entiny_;
	}


private:
	sol::state lua_;
	std::string path_;
	size_t entiny_ = -1;
};

#endif // !__SCRIPT_DATA_H__
