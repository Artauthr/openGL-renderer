#include "TestBatchColors.h"

namespace test {
	TestBatchColors::TestBatchColors()
	{
        float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
         -0.5f, -0.5f, 0.0f,  // bottom left
         -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        m_Shader = std::make_unique<Shader>("res/shaders/vertex_basic.shader", "res/shaders/fragment_basic.shader");




        //VertexBufferLayout layout;
        //layout.AddFloat(3);
        //layout.AddFloat(2);


        glBindVertexArray(VAO);
	}

	TestBatchColors::~TestBatchColors()
	{
	}

	void TestBatchColors::OnUpdate(float deltaTime)
	{
	}

	void TestBatchColors::OnRender()
	{
        m_Shader->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void TestBatchColors::OnImGuiRender()
	{
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
} 