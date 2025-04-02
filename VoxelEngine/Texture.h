#pragma once
#include <string>
class Texture
{
public:
	static Texture* get(const std::string& fileName);

public:
	void setActive();
	void setTexUnit(int unit);
private:
	Texture(const std::string& fileName);

	unsigned int _id = 0;
};