#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__ 1

#include <unordered_map>
#include <string>
#include <memory>
#include "Motor/common/dev/program.h"

class ProgramManager {
public:
	ProgramManager();
	~ProgramManager();

	void loadProgram(const std::string& name, const std::string& ver, const std::string& frag);

	std::shared_ptr<Program> Get(const std::string& name);

private:
	std::unordered_map <std::string, std::shared_ptr<Program>> programs_;
};

#endif // !__PROGRAM_MANAGER_H__
