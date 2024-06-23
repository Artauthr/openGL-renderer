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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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
        // Position           // Color            // Texture Coords
        // Front face
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 0
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 1
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 3

        // Back face
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 4
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 5
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 6
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f  // 7
    };

    unsigned int indices[] = {
         0, 1, 2, 2, 3, 0,  // Front face
         4, 5, 6, 6, 7, 4,  // Back face
         0, 4, 7, 7, 3, 0,  // Left face
         1, 5, 6, 6, 2, 1,  // Right face
         3, 2, 6, 6, 7, 3,  // Top face
         0, 1, 5, 5, 4, 0   // Bottom face
    };


    Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    VertexArrayObject vao;
    vao.Bind();

    VertexBuffer vb;
    vb.Bind();
    vb.SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

    vb.AttribPointer(0, 3, 8 * sizeof(float), (void*)0);
    vb.AttribPointer(1, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vb.AttribPointer(2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    IndexBuffer ib;
    ib.Bind();
    ib.SetBufferData(sizeof(indices), indices, GL_STATIC_DRAW);

    Texture texture("res/textures/container.jpg");

    shader.Bind();
    shader.SetUniform1i("tex1", 0);
   

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.Bind();
        shader.Bind();
   

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
        shader.SetUniformMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetUniformMat4("view", view);


        vao.Bind();

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.SetUniformMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Info");
        ImGui::Text("deltaTime: %.3f ms", deltaTime * 1000.0f);
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

