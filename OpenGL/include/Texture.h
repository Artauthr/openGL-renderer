#pragma once

#include <string>
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>


class Texture
{
public:
	Texture(const std::string path);
	~Texture();
	void Bind(unsigned int unit = 0) const;
	void Unbind() const;
private:
	GLuint ID;
	std::string m_Path; //debugging purposes
};

