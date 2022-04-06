#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class VertexBuffer
{
private:
	unsigned int m_ID;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const glm::vec2* data, unsigned int size);
	VertexBuffer(const glm::vec3* data, unsigned int size);
	VertexBuffer(int num);
	~VertexBuffer();
	void Bind();
	void UnBind();
	void getData(const void* data, unsigned int size);
};

#endif
