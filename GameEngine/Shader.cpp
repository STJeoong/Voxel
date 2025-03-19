#include "Shader.h"
#include <fstream>
#include <sstream>
#include <fmt/core.h>

#pragma region public
Shader::Shader(const std::string& vert, const std::string& frag)
{
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	this->compile(vert, _vertexShader);
	this->compile(frag, _fragShader);

	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragShader);
	glLinkProgram(_program);

	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
	{
		glUseProgram(_program);
		return;
	}

	char buf[512] = {};
	glGetProgramInfoLog(_program, 511, nullptr, buf);
	throw std::runtime_error(fmt::format("GLSL Link Status:\n{}", buf));
}
Shader::~Shader()
{
	glDeleteProgram(_program);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragShader);
}
#pragma endregion


#pragma region private
void Shader::compile(const std::string& fileName, GLuint shader)
{
	std::ifstream ifstream(fileName);
	if (!ifstream.is_open())
		throw std::runtime_error(fmt::format("Shader file not found : {}", fileName));

	std::stringstream sstream;
	sstream << ifstream.rdbuf();
	std::string contents = sstream.str();
	const char* contentsChar = contents.c_str();

	glShaderSource(shader, 1, &(contentsChar), nullptr);
	glCompileShader(shader);

	// check compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return;

	char buf[512] = {};
	glGetShaderInfoLog(shader, 511, nullptr, buf);
	throw std::runtime_error(fmt::format("GLSL Compile Failed : \n{}", buf));
}
#pragma endregion