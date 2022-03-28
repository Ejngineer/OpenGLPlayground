#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <string>

class Texture
{
private:
	unsigned int m_ID;
	std::string type;
public:
	Texture();
	Texture(int num);
	~Texture();
	void LoadTexture(const char* TexturePath);
	void ActivateTexture(int tex_idx);
	void EnableVertFlip();
	void DisableVertFlip();
	void Bind();
	void unBind();
};

#endif