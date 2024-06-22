#pragma once

#include "GL/glew.h"
#include "imgui/imgui.h"

#include <memory>

#include "Test.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	class TestTextures : public Test
	{
	public:
		TestTextures();
		~TestTextures();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::vec3 m_Translation;
	};
}
