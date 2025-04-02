#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include <format>
#include <map>

static std::map<std::string, Texture*> s_texture;

#pragma region public static
Texture* Texture::get(const std::string& fileName)
{
	if (s_texture.find(fileName) == s_texture.end())
		s_texture[fileName] = new Texture(fileName);
	return s_texture[fileName];
}
#pragma endregion



#pragma region public
Texture::Texture(const std::string& fileName)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channel;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channel, 0);
	if (data == nullptr)
		throw std::runtime_error("Failed to load texture");

	if (fileName.find(".png") != std::string::npos)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (fileName.find(".jpg") != std::string::npos)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		throw std::runtime_error(std::format("Not supported texture file extension : {}", fileName));
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}
void Texture::setActive()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}
void Texture::setTexUnit(int unit)
{
	switch (unit)
	{
	case 0: glActiveTexture(GL_TEXTURE0); break;
	case 1: glActiveTexture(GL_TEXTURE1); break;
	case 2: glActiveTexture(GL_TEXTURE2); break;
	case 3: glActiveTexture(GL_TEXTURE3); break;
	}
	this->setActive();
}
#pragma endregion