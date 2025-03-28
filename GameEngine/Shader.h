#pragma once
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
	void setMat4f(const std::string& name, const float* value);
private:
	void compile(const std::string& fileName, unsigned int shader);

	unsigned int _vertexShader = 0;
	unsigned int _fragShader = 0;
	unsigned int _id = 0;
};