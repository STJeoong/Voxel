#include "Shader.h"
#include <fstream>
#include <sstream>
#include <format>

#pragma region public
Shader::Shader(const std::string& vert, const std::string& frag)
{
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	this->compile(vert, _vertexShader);
	this->compile(frag, _fragShader);

	_id = glCreateProgram();
	glAttachShader(_id, _vertexShader);
	glAttachShader(_id, _fragShader);
	glLinkProgram(_id);

	GLint status;
	glGetProgramiv(_id, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
	{
		glDeleteShader(_vertexShader);
		glDeleteShader(_fragShader);
		return;
	}

	char buf[512] = {};
	glGetProgramInfoLog(_id, 511, nullptr, buf);
	throw std::runtime_error(std::format("GLSL Link Status:\n{}", buf));
}
Shader::~Shader()
{
	glDeleteProgram(_id);
}
void Shader::setActive()
{
	glUseProgram(_id);
}
void Shader::setFloat(const std::string& name, float value)
{
	this->setActive();
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1f(location, value);
}
void Shader::setBool(const std::string& name, bool value)
{
	this->setActive();
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1i(location, static_cast<int>(value));
}
void Shader::setInt(const std::string& name, int value)
{
	this->setActive();
	int location = glGetUniformLocation(_id, name.c_str());
	glUniform1i(location, value);
}
void Shader::setMat4f(const std::string& name, const float* value)
{
	this->setActive();
	int location = glGetUniformLocation(_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_TRUE, value);
}
#pragma endregion


#pragma region private
void Shader::compile(const std::string& fileName, GLuint shader)
{
	std::ifstream ifstream(fileName);
	if (!ifstream.is_open())
		throw std::runtime_error(std::format("Shader file not found : {}", fileName));

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
	throw std::runtime_error(std::format("GLSL Compile Failed : \n{}", buf));
}
#pragma endregion