#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();
	void Bind();
	void Unbind();
	void SetBufferData(GLint size, const void* data, GLenum usage);
private:
	GLuint ID;
};
