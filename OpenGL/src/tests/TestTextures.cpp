#include "TestTextures.h"


namespace test {
    TestTextures::TestTextures()
        : m_Translation(200, 200, 0),
          m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)))
	{
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f, //0
            200.0f, 100.0f, 1.0f, 0.0f, //1
            200.0f, 200.0f, 1.0f, 1.0f, //2
            100.0f, 200.0f, 0.0f, 1.0f, //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        m_VAO = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.AddFloat(2);
        layout.AddFloat(2);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_Shader = std::make_unique<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_Texture = std::make_unique<Texture>("res/textures/icon.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTextures::~TestTextures()
	{
	}
	void TestTextures::OnUpdate(float deltaTime)
	{
	}
	void TestTextures::OnRender()
	{
        Renderer renderer;
        m_Texture->Bind();
        m_Shader->Bind();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 mvp = m_Proj * m_View * model;

        m_Shader->SetUniformMat4("u_MVP", mvp);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
	void TestTextures::OnImGuiRender()
	{
        ImGui::SliderFloat3("float", &m_Translation.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
