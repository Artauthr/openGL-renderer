#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
	:ID(0)
{
	glGenBuffers(1, &ID);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &ID);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetBufferData(GLint size, const void* data, GLenum usage)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

}
