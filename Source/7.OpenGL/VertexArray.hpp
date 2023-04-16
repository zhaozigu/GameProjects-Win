#pragma once
#include <vector>

class VertexArray
{
public:
    VertexArray(const std::vector<float>& verts, const std::vector<unsigned int>& indices);
    ~VertexArray();

    // 激活顶点序列（你才可以绘制它）
    void SetActive();

    unsigned int GetNumIndices() const { return mNumIndices; }
    unsigned int GetNumVerts() const { return mNumVerts; }

private:
    // 顶点缓冲区的顶点数量
    unsigned int mNumVerts;
    // 索引缓冲区的数量
    unsigned int mNumIndices;
    // 顶点缓冲区的 OpenGL ID
    unsigned int mVertexBuffer;
    // 索引缓冲区的 OpenGL ID
    unsigned int mIndexBuffer;
    // 顶点序列对象 的 OpenGL ID
    unsigned int mVertexArray;
};
