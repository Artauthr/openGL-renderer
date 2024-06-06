#include "Shader.h"


Shader::Shader(const std::string &vsFilePath, const std::string &fsFilePath)
	:m_RendererID(0)
{
    std::string vsParsed = ParseShaderSourceFile(vsFilePath);
    std::string fsParsed = ParseShaderSourceFile(fsFilePath);

    m_RendererID = CreateShaderProgram(vsParsed, fsParsed);
}

std::string Shader::ParseShaderSourceFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

unsigned int Shader::CompileShader(const GLuint type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);


    // check status
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(id, 1024, &log_length, message);

        std::cout << "Failed to compile shader of type: " << type << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &log_length, message);
        std::cout << "Failed linking program: " << message << std::endl;
    }


    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    unsigned int location = GetUniformLocation(name);

    if (location == -1)
    {
        //whoops
        std::cerr << "Couldn't find uniform location by name: " << name << std::endl;
    }

    glUniform4f(location, v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string& name)
{
    return glGetUniformLocation(m_RendererID, name.c_str());
}
