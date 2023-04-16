#include "VertexArray.hpp"
#include "GL/glew.h"

VertexArray::VertexArray(const std::vector<float> &verts, const std::vector<unsigned int> &indices)
    : mNumVerts(verts.size() / 3), mNumIndices(indices.size())
{
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    // 创建顶点缓冲区
    glGenBuffers(1, &mVertexBuffer);
    // 打算使用这个缓冲区作为顶点缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    // 复制顶点数据到序列缓冲对象
    glBufferData(GL_ARRAY_BUFFER,
                 verts.size() * sizeof(float), // 复制的字节数量
                 verts.data(),                 // 源
                 GL_STATIC_DRAW                // 使用方式
    );

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 指定顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
    glBindVertexArray(mVertexArray);
}
