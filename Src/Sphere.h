#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "VertexArray.h"
#include "VertexBuffer.h"

class Sphere
{
private:
	VertexBuffer VBO;
	VertexArray VAO;
public:
	Sphere(int radius);
	void Draw();
};


#endif
