#pragma once
#include <string>
class Texture
{
public:
	Texture(const std::string& fileName, int internalFormat, int fileFormat);
	void setActive();
	void setTexUnit(int unit);
private:

	int _width = 0;
	int _height = 0;
	int _channel = 0;
	unsigned int _id = 0;
};