#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	void Bind();
	void Unbind();
	void SetBufferData(GLint size, const void* data, GLenum usage);
	void AttribPointer(GLuint location, GLsizei attribSize, int stride, unsigned int offsetPtr);
private:
	unsigned int ID;
};