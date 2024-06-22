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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
//
//#include "tests/TestClearColor.h"
//#include "tests/Test.h"
//#include "tests/TestTextures.h"
//#include "tests/TestBatchColors.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    if (severity < GL_DEBUG_SEVERITY_HIGH)
    {
       // return;
    }

    std::cerr << "OpenGL Debug Message:" << std::endl;
    std::cerr << "Source: " << source << std::endl;
    std::cerr << "Severity: " << severity << std::endl;
    std::cerr << "Message: " << message << std::endl;
    std::cerr << "==========================" << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        __debugbreak();
    }
}


int main(void)
{
    GLFWwindow* window;


    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;\
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglDebugCallback, nullptr);


    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.4f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.1f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 0.7f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    GLuint VB;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;

    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    std::string path = "res/textures/container.jpg";

    unsigned char* m_LocalBuffer = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (m_LocalBuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    } 
    else
    {
        __debugbreak();
    }

    stbi_image_free(m_LocalBuffer);

    // textures end


    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    shader.Bind();
    shader.SetUniform1i("tex1", 0);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    shader.SetUniformMat4("projection", projection);


    // setup ImGui 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 


    ImGui::StyleColorsDark();
    
    // setup imGui Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
    /////// 

    //float rot = 45.0f;
  //  glm::vec3 scl = glm::vec3();

   // glm::mat4 projection;
   // projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

   // test::Test* currentTest;
   // test::TestMenu* menu = new test::TestMenu(currentTest);
   // currentTest = menu;
   //
   // menu->RegisterTest<test::TestClearColor>("Clear Color");
   // menu->RegisterTest<test::TestTextures>("Textures");
   // menu->RegisterTest<test::TestBatchColors>("Batch colors");

    glm::vec3 viewTranslate = glm::vec3(0.0f, 0.0f, -3.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        shader.Bind();


        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);


        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, viewTranslate);
   

        shader.SetUniformMat4("model", model);
        shader.SetUniformMat4("view", view);
     

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        ImGui::Begin("Controls");
       //ImGui::SliderFloat("Rotation", &rot, -360.0, 360.0);
        ImGui::SliderFloat3("View", glm::value_ptr(viewTranslate), -10, 10);
        ImGui::End();
       
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();



       // ImGui_ImplOpenGL3_NewFrame();
       // ImGui_ImplGlfw_NewFrame();
       // ImGui::NewFrame();
       //
       // if (currentTest)
       // {
       //     currentTest->OnUpdate(0.0f);
       //     currentTest->OnRender();
       //     ImGui::Begin("Test");
       //     if (currentTest != menu && ImGui::Button("<-"))
       //     {
       //         delete currentTest;
       //         currentTest = menu;
       //     }
       //     currentTest->OnImGuiRender();
       //     ImGui::End();
       // }
       //
       // ImGui::Render();
       // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VB);


   // ImGui_ImplOpenGL3_Shutdown();
   // ImGui_ImplGlfw_Shutdown();
   // ImGui::DestroyContext();
  
    glfwTerminate();

    return 0;
}