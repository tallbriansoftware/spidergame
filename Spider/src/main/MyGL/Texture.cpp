#include "MyGL/Texture.h"

#include <string>

#include "stb_image/std_image.h"

#include "MyGL/GLErrors.h"
#include "GL/glew.h"


Texture::Texture(const std::string& path)
    : m_rendererId(0)
    , m_filepath(path)
    , m_localBuffer(nullptr)
    , m_width(0)
    , m_height(0)
    , m_BPP(0)
{
    //stbi_set_flip_vertically_on_load(1);
    m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_rendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer)
    {
        stbi_image_free(m_localBuffer);
        m_localBuffer = nullptr;
    }
}


Texture::~Texture()
{
    if (m_localBuffer)
    {
        stbi_image_free(m_localBuffer);
        m_localBuffer = nullptr;
    }
    glDeleteTextures(1, &m_rendererId);
}


void Texture::Bind(unsigned int slot /* = 0 */) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}


void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::GetWidth()
{
    return m_width;
}

int Texture::GetHeight()
{
    return m_height;
}
