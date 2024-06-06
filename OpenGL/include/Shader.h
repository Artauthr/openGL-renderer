#pragma once
#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
	unsigned int m_RendererID;
public:
	Shader(const std::string &vsFilePath, const std::string &fsFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	std::string ParseShaderSourceFile(const std::string& filepath);
	unsigned int CompileShader(const GLuint type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};