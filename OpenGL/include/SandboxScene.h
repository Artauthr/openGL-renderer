#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class SandboxScene
{
public:
	SandboxScene() {};
	~SandboxScene() {};
	virtual void Render() {};
	virtual void ImGuiRender() {};
	virtual void MouseCallback(double xpos, double ypos) {};
	virtual void ScrollCallback(double xoffset, double yoffset) {};
	virtual void ProcessInput(GLFWwindow*, float deltaTime) {};
};