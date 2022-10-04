#include "MyGL/VertexArray.h"


#include "MyGL/VertexBuffer.h"
#include "MyGL/VertexBufferLayout.h"

#include "MyGL/GLErrors.h"
#include "GL/glew.h"


VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_rendererId));
    GLCall(glBindVertexArray(m_rendererId));
}


VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    __int64 offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count,
            element.type,
            element.normalized,
            layout.GetStride(),
            (const void*)offset));

        offset += (__int64)element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}


void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_rendererId));
}


void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
