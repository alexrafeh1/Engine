#include "Motor/common/manager/program_manager.h"

ProgramManager::ProgramManager()
{

}

ProgramManager::~ProgramManager()
{
}

void ProgramManager::loadProgram(const std::string& name, const std::string& ver, const std::string& frag)
{
	auto program = std::make_shared<Program>();
	std::optional<Shader> vs = Shader::uploadShader(ver);
	std::optional<Shader> fs = Shader::uploadShader(frag);
	if (!vs)
	{
		assert(false && "vertex failed to load");
	}

	if (!fs)
	{
		assert(false && "fragment failed to load");
	}

	program->AttachShader(vs.value());
	program->AttachShader(fs.value());
	program->Link();

	programs_[name] = program;
}

std::shared_ptr<Program> ProgramManager::Get(const std::string& name)
{
	auto it = programs_.find(name);

	if (it == programs_.end())
		return nullptr;

	return it->second;
}