#pragma once
#include "vector"
#include "GL/glew.h"
#include <iostream>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(unsigned int c, unsigned int t, unsigned char n) :
		type(t), count(c), normalized(n)
	{
		std::cout << "VertexBufferElement created: count = " << count
			<< ", type = " << type
			<< ", normalized = " << (int)normalized << std::endl;
	}

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		return 0;
	}
};


class VertexBufferLayout
{
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;

	void Push(unsigned int type, unsigned int count, unsigned char normalized)
	{
		m_Elements.push_back({ count, type, normalized });
		m_Stride += count * VertexBufferElement::GetSizeOfType(type);
	};

public:
	VertexBufferLayout() :
		m_Stride(0) { }

	void AddFloat(unsigned int count) { Push(GL_FLOAT, count, GL_FALSE); }
	void AddUnsignedInt(unsigned int count) { Push(GL_UNSIGNED_INT, count, GL_FALSE); }
	void AddUnsignedByte(unsigned int count) { Push(GL_UNSIGNED_BYTE, count, GL_TRUE); }

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };
};
