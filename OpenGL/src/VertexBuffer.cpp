#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
	:ID(0)
{
	glGenBuffers(1, &ID);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &ID);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetBufferData(GLint size, const void* data, GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::AttribPointer(GLuint location, GLsizei attribSize, int stride, const void* offsetPtr)
{
	glVertexAttribPointer(location, attribSize, GL_FLOAT, GL_FALSE, stride, offsetPtr);
	glEnableVertexAttribArray(location);
}
