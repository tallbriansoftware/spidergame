#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_rendererId;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

