#include "Motor/common/dev/program.h"

Program::Program()
{
	program_ = glCreateProgram();
}

Program::~Program()
{
	if (glIsProgram(program_)) {
		glDeleteProgram(program_);
	}
}

Program::Program(Program&& other) : 
	program_(other.program_)
{
	other.program_ = 0;
}

Program& Program::operator=(Program&& other)
{
	if (this != &other) {
		if (glIsProgram(program_))
			glDeleteProgram(program_);
		program_ = other.program_;
		other.program_ = 0;
	}
	return *this;
}



void Program::AttachShader(const Shader& shader)
{
	glAttachShader(program_, shader.Get());
}

void Program::Link()
{
	glLinkProgram(program_);
	GLint success;
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program_, 512, nullptr, infoLog);
		printf("Error linking program: %s\n", infoLog);
	}
}


void Program::UseProgram()
{
	glUseProgram(program_);
}


unsigned int Program::GetUniformLocation(const char* name) {
	return glGetUniformLocation(program_, name);
}


void Program::SetUniform(const char* name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Program::SetUniform(const char* name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Program::SetUniform(const char* name, bool value)
{
	glUniform1i(GetUniformLocation(name), value ? 1 : 0);
}

void Program::SetUniform(const char* name, const glm::vec2& value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Program::SetUniform(const char* name, const glm::vec3& value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Program::SetUniform(const char* name, const glm::vec4& value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Program::SetUniform(const char* name, const glm::mat3& value)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Program::SetUniform(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}




