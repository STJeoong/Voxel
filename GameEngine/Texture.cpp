#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

static bool s_flip = false;

#pragma region public
Texture::Texture(const std::string& fileName, int internalFormat, int fileFormat)
{
	if (s_flip == false)
	{
		s_flip = true;
		stbi_set_flip_vertically_on_load(s_flip);
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char* data = stbi_load(fileName.c_str(), &_width, &_height, &_channel, 0);
	if (data == nullptr)
		throw std::runtime_error("Failed to load texture");

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, fileFormat, GL_UNSIGNED_BYTE, data);
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