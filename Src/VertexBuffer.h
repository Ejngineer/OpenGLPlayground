#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <glad/glad.h>


class VertexBuffer
{
private:
	unsigned int m_ID;
	void getData(const void* data, unsigned int size);
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(int num);
	~VertexBuffer();
	void Bind();
	void UnBind();
};

#endif
