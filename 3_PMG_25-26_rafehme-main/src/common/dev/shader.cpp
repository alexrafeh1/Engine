#include "Motor/common/dev/shader.h"


Shader::Shader()
{
	shader_ = 0;
}

Shader::~Shader()
{
	if (glIsShader(shader_)) {
		glDeleteShader(shader_);
	}
}

Shader::Shader(Shader&& other) 
	: shader_(other.shader_)
{
	other.shader_ = 0;
}

Shader& Shader::operator=(Shader&& other)
{
	if (this != &other) {
		if (glIsShader(shader_))
			glDeleteShader(shader_);
		shader_ = other.shader_;
		other.shader_ = 0;
	}
	return *this;
}

std::optional<std::string> Slurp(std::string path) {
	FILE* in_file = nullptr;
	fopen_s(&in_file, path.c_str(), "rb");

	if (!in_file) {
		return std::nullopt;
	}

	fseek(in_file, 0, SEEK_END);
	long size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	std::string content(size, '\0');
	fread(content.data(), 1, size, in_file);
	fclose(in_file);

	return content;
}

std::optional<Shader> Shader::uploadShader(std::string_view path)
{
	Shader shader;
	std::string p = std::string(path);
	std::optional<std::string> file = Slurp(p);

	if (!file.has_value()) {
		return std::nullopt;
	}

	const std::string& source = file.value();
	const char* data = source.c_str();

	auto pos = p.find_last_of('.');
	if (pos == std::string::npos)
		return std::nullopt;

	std::string ext = p.substr(pos + 1);

	if (ext == "vs") {
		shader.shader_ = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (ext == "fs") {
		shader.shader_ = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else if (ext == "gs") {
		shader.shader_ = glCreateShader(GL_GEOMETRY_SHADER); 
	}
	else {
		return std::nullopt;
	}

	glShaderSource(shader.shader_, 1, &data, nullptr);
	glCompileShader(shader.shader_);

	GLint success;
	glGetShaderiv(shader.shader_, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader.shader_, 512, nullptr, infoLog);
		printf("Error compiling shader %s: %s\n", p.c_str(), infoLog);
		glDeleteShader(shader.shader_); 
		return std::nullopt;
	}

	return shader;
}






