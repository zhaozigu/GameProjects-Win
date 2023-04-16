#pragma once
#include <string>
#include <GL/glew.h>

class GLShader
{
public:
    GLShader();
    ~GLShader();
    // 加载给定名称的顶点/片段着色器
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();
    // 作为激活的 Shader 程序
    void SetActive();
private:
    // 尝试编译指定的 Shader
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);
    
    // 测试是否成功编译
    bool IsCompiled(GLuint shader);
    // 测试顶点/片段着色器的连接
    bool IsValidProgram();
private:
    // 存储Shader对象的ID
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
