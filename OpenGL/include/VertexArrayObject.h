#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();
	void Bind();
	void Unbind();
private:
	unsigned int ID;
};