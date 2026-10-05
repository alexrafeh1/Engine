#ifndef __SHADER_H__
#define __SHADER_H__ 1

#include <glad/gl.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <string.h>
#include "../deps/glm/vec2.hpp"
#include "../deps/glm/vec3.hpp"
#include "../deps/glm/vec4.hpp"
#include "../deps/glm/mat4x4.hpp"
#include <optional>



class Shader {

public:
	Shader();
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	static std::optional<Shader> uploadShader(std::string_view  path);

	unsigned int Get() const {
		return shader_;
	}

private:
	unsigned int shader_;
};

#endif // !__SHADER_H__
