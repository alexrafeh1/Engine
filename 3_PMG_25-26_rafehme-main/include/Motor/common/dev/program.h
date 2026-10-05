#ifndef __PROGRAM_H__
#define __PROGRAM_H__ 1

#include <optional>
#include <glad/gl.h>
#include <string>
#include "../deps/glm/vec2.hpp"
#include "../deps/glm/vec3.hpp"
#include "../deps/glm/vec4.hpp"
#include "../deps/glm/mat3x3.hpp"
#include "../deps/glm/mat4x4.hpp"
#include "Motor/common/dev/shader.h"

class Program {
public:
	Program();
	~Program();

	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	Program(Program&&);
	Program& operator=(Program&&);


	void AttachShader(const Shader& shader);
	void Link();
	void UseProgram();


	void SetUniform(const char* name, int value);
	void SetUniform(const char* name, float value);
	void SetUniform(const char* name, bool value);
	void SetUniform(const char* name, const glm::vec2& value);
	void SetUniform(const char* name, const glm::vec3& value);
	void SetUniform(const char* name, const glm::vec4& value);
	void SetUniform(const char* name, const glm::mat3& value);
	void SetUniform(const char* name, const glm::mat4& value);

private:
	unsigned int GetUniformLocation(const char* name);
	unsigned int program_;
};


#endif // !__PROGRAM_H__
