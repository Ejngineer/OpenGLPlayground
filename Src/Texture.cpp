#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture()
{
	glGenTextures(1, &m_ID);
}

Texture::Texture(int num)
{
	glGenTextures(num, &m_ID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::LoadTexture(const char* TexturePath)
{
	int width, height, nrComponents;
	unsigned char* data = stbi_load(TexturePath, &width, &height, &nrComponents, 0);
	if (data)
	{

		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::ActivateTexture(int tex_idx)
{
	glActiveTexture(GL_TEXTURE0 + tex_idx);
}

void Texture::EnableVertFlip()
{
	stbi_set_flip_vertically_on_load(true);
}

void Texture::DisableVertFlip()
{
	stbi_set_flip_vertically_on_load(false);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
