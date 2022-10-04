#include "MyGL/VertexBuffer.h"

#include "MyGL/GLErrors.h"
#include "GL/glew.h"


VertexBuffer::VertexBuffer(const void* data, size_t size)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));

}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
