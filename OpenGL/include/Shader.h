#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_LocationMap;
public:
	Shader(const std::string& vsFilePath, const std::string& fsFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	void SetUniformVec3(const std::string& name, const glm::vec3& vec);
	void SetUniformVec3(const std::string& name, float r, float g, float b);

private:
	std::string ParseShaderSourceFile(const std::string& filepath);
	unsigned int CompileShader(const GLuint type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};