#include "GLShader.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <SDL_log.h>

GLShader::GLShader()
    : mShaderProgram(0), mVertexShader(0), mFragShader(0)
{
}

GLShader::~GLShader()
{
}

bool GLShader::Load(const std::string &vertName, const std::string &fragName)
{
    // 编译顶点和像素着色器
    if (!CompileShader(vertName,
                       GL_VERTEX_SHADER,
                       mVertexShader) ||
        !CompileShader(fragName,
                       GL_FRAGMENT_SHADER,
                       mFragShader))
    {
        return false;
    }

    // 将顶点和片段着色器连接
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    // 验证连接
    if (!IsValidProgram())
    {
        return false;
    }

    return true;
}

void GLShader::Unload()
{
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

void GLShader::SetActive()
{
    glUseProgram(mShaderProgram);
}

bool GLShader::CompileShader(const std::string &fileName, GLenum shaderType, GLuint &outShader)
{
    // 打开文件
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open())
    {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char *contentsChar = contents.c_str();

        // 创建指定类型的 Shader
        outShader = glCreateShader(shaderType);
        // 设置代码内容并尝试编译
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader))
        {
            SDL_Log("编译 shader 失败 %s", fileName.c_str());
            return false;
        }
    }
    else
    {
        SDL_Log("Shader 文件没找到: %s", fileName.c_str());
        return false;
    }

    return true;
}

constexpr int kBufferSize = 1024;

bool GLShader::IsCompiled(GLuint shader)
{
    GLint status;
    // 询问编译状态
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[kBufferSize];
        memset(buffer, 0, kBufferSize);
        glGetShaderInfoLog(shader, kBufferSize - 1, nullptr, buffer);
        SDL_Log("GLSL 编译失败:\n%s", buffer);
        return false;
    }

    return true;
}

bool GLShader::IsValidProgram()
{
    GLint status;
    // 询问连接状态
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[kBufferSize];
        memset(buffer, 0, kBufferSize);
        glGetProgramInfoLog(mShaderProgram, kBufferSize - 1, nullptr, buffer);
        SDL_Log("GLSL 连接状态:\n%s", buffer);
        return false;
    }

    return true;
}
