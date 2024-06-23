#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
	:ID(0)
{
	glGenVertexArrays(1, &ID);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(ID);
}

void VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}
