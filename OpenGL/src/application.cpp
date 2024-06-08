#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"


void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL Debug Message:" << std::endl;
    std::cerr << "Source: " << source << std::endl;
    //std::cerr << "Type: " << type << std::endl;
    //std::cerr << "ID: " << id << std::endl;
    std::cerr << "Severity: " << severity << std::endl;
    std::cerr << "Message: " << message << std::endl;
    std::cerr << "==========================" << std::endl;
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        -0.5f, -0.5f, 0.0f, 0.0f, //0
         0.5f, -0.5f, 1.0f, 0.0f, //1
         0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f,  0.5f, 0.0f, 1.0f, //3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

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

    Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.1f, 1.0f);


    shader.Unbind();
    vb.Unbind();
    va.Unbind();
    ib.Unbind();

    Renderer renderer;


    Texture texture("res/textures/icon.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);



    while (!glfwWindowShouldClose(window))
    {
      
        renderer.Clear();     
        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(window);

     
        glfwPollEvents();
    }

  

  
    glfwTerminate();

    return 0;
}