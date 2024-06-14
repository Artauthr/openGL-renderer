#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"



void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    if (severity < GL_DEBUG_SEVERITY_HIGH)
    {
        return;
    }

    std::cerr << "OpenGL Debug Message:" << std::endl;
    std::cerr << "Source: " << source << std::endl;
    std::cerr << "Severity: " << severity << std::endl;
    std::cerr << "Message: " << message << std::endl;
    std::cerr << "==========================" << std::endl;

   // if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
   //     __debugbreak();
   // }
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* context */
    glfwMakeContextCurrent(window);

  

    // glew innit?
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << (stderr, "Error: %s\n", glewGetErrorString(err)) << std::endl;
        return 0;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(openglDebugCallback, nullptr);


    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    //triangle arr
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


    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
 

    ImGui::StyleColorsDark();

    // setup imGui Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
    /////// 


    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    IndexBuffer ib(indices, 6);

    VertexBufferLayout layout;
    layout.AddFloat(2);
    layout.AddFloat(2);

    va.AddBuffer(vb, layout);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

    
    Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.1f, 1.0f);
  

    Texture texture("res/textures/icon.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    vb.Unbind();
    va.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    glm::vec3 translation(200, 200, 0);


    while (!glfwWindowShouldClose(window))
    {
      
        renderer.Clear();     

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;

        shader.SetUniformMat4("u_MVP", mvp);

        renderer.Draw(va, ib, shader);

        {
            ImGui::SliderFloat3("float", &translation.x, 0.0f, 960.0f);        
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);

     
        glfwPollEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  
    glfwTerminate();

    return 0;
}