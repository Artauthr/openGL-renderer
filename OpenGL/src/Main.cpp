#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObject.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

static void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
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
        throw std::runtime_error("Failed to init GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    /* context */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
  //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //////// setup ImGui 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
    /////// 


    // glew innit?
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << (stderr, "Error: %s\n", glewGetErrorString(err)) << std::endl;
        throw std::runtime_error("Failed to init GLEW");
    }

    // enable shit
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);


    // debugging
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglDebugCallback, nullptr);

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
        // Position          
        // Front face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Back face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };

    unsigned int indices[] = {
         0, 1, 2, 2, 3, 0,  // Front face
         4, 5, 6, 6, 7, 4,  // Back face
         0, 4, 7, 7, 3, 0,  // Left face
         1, 5, 6, 6, 2, 1,  // Right face
         3, 2, 6, 6, 7, 3,  // Top face
         0, 1, 5, 5, 4, 0   // Bottom face
    };

    Shader lightSource("res/shaders/lighting/vertex_light_src.shader", "res/shaders/lighting/frag_light_src.shader");
    Shader lightedObject("res/shaders/lighting/vert_lighted_obj.shader", "res/shaders/lighting/frag_lighted_obj.shader");

    VertexArrayObject cubeVAO;
    cubeVAO.Bind();

    VertexBuffer cubeVB;
    cubeVB.Bind();
    cubeVB.SetBufferData(sizeof(vertices), &vertices, GL_STATIC_DRAW);
    cubeVB.AttribPointer(0, 3, sizeof(float) * 3, 0);

    IndexBuffer cubeIB;
    cubeIB.Bind();
    cubeIB.SetBufferData(sizeof(indices), &indices, GL_STATIC_DRAW);

    cubeVAO.Unbind();


    //// lighter
    VertexArrayObject lightSourceVAO;
    lightSourceVAO.Bind();

    VertexBuffer lightSourceVB;
    lightSourceVB.Bind();
    lightSourceVB.SetBufferData(sizeof(vertices), &vertices, GL_STATIC_DRAW);
    lightSourceVB.AttribPointer(0, 3, sizeof(float) * 3, 0);

    IndexBuffer lightSourceIB;
    lightSourceIB.Bind();
    lightSourceIB.SetBufferData(sizeof(indices), &indices, GL_STATIC_DRAW);

    ////

    lightedObject.Bind();
    lightedObject.SetUniformVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    //lightedObject.SetUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    lightedObject.Unbind();
   

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightedObject.Bind();
        lightedObject.SetUniformVec3("lightColor", lightColor);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightedObject.SetUniformMat4("projection", projection);
        lightedObject.SetUniformMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightedObject.SetUniformMat4("model", model);

    
        cubeVAO.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    
        lightSource.Bind();
        lightSource.SetUniformMat4("projection", projection);
        lightSource.SetUniformMat4("view", view);
      
        // world transformation
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightSource.SetUniformMat4("model", model);
      
        lightSourceVAO.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Info");
        ImGui::Text("deltaTime: %.3f ms", deltaTime * 1000.0f);
        ImGui::SliderFloat3("Light color", glm::value_ptr(lightColor), 0.0f, 1.0f);
        ImGui::End();
        
        
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

