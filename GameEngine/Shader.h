#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader(const std::string& vert, const std::string& frag);
	~Shader();
	void setActive();
	void setFloat(const std::string& name, float value);
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
private:
	void compile(const std::string& fileName, GLuint shader);

	GLuint _vertexShader = 0;
	GLuint _fragShader = 0;
	GLuint _id = 0;
};