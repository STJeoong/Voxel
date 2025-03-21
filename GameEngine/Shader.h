#pragma once
#include <glad/glad.h>
#include <string>
class Shader
{
public:
	Shader(const std::string& vert, const std::string& frag);
	~Shader();
	void setActive();
private:
	void compile(const std::string& fileName, GLuint shader);

	GLuint _vertexShader = 0;
	GLuint _fragShader = 0;
	GLuint _program = 0;
};