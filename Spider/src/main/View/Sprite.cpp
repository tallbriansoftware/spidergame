#include "View/Sprite.h"

#include "MyGL/Texture.h"
#include "Model/CardModel.h"
#include "View/SpriteFactory.h"

#include <memory>


Sprite::Sprite(SpriteType type, SpriteFactory& factory)
    : m_texture(nullptr)
    , m_vertexBuffer(nullptr)
    , m_vao(nullptr)
    , m_indexBuffer(nullptr)
    , m_shader(nullptr)
    , m_position(glm::vec3(0.0f))
    , m_size(glm::vec3(0.0f))
    , m_angle(0.0f)
    , m_type(type)
    , m_factory(factory)
{
}


void Sprite::LoadGraphics(
    const std::shared_ptr<Texture>& texture,
    const std::shared_ptr<VertexBuffer>& vertexBuffer,
    const std::shared_ptr<VertexArray>& vao,
    const std::shared_ptr<IndexBuffer>& indexBuffer,
    const std::shared_ptr<Shader>& shader,
    const glm::vec3& size)
{
    m_texture = texture;
    m_vertexBuffer = vertexBuffer;
    m_vao = vao;
    m_indexBuffer = indexBuffer;
    m_shader = shader;
    m_size = size;
}


SpriteType Sprite::getType() const
{
    return m_type;
}


// position of the upper left corner
void Sprite::setPosition(const glm::vec2& pos2)
{
    glm::vec3 pos3 = glm::vec3(pos2, 0.0f);
    m_position = pos3;
}
void Sprite::setPosition(const glm::vec3& position)
{
    m_position = position;
}


// position of the upper left corner
const glm::vec3& Sprite::getPosition() const
{
    return m_position;
}
glm::vec2 Sprite::getTopLeft() const
{
    return glm::vec2(m_position.x, m_position.y);
}


void Sprite::setSize(const glm::vec2& size)
{
    m_size = glm::vec3(size, 0.0f);
}

const glm::vec3& Sprite::getSize() const
{
    return m_size;
}


void Sprite::setAngleDegrees(float angle)
{
    double pi = 3.14159265358979311600;
    m_angle = static_cast<float>(angle * (pi / 180));
}

float Sprite::getAngleDegrees() const
{
    double pi = 3.14159265358979311600;
    return static_cast<float>(m_angle / (pi / 180));
}


void Sprite::setAngleRadians(float angle)
{
    m_angle = angle;
}

float Sprite::getAngleRadians() const
{
    return m_angle;
}


const Texture* Sprite::getTexture() const
{
    return m_texture.get();
}


const VertexBuffer* Sprite::getVertexBuffer() const
{
    return m_vertexBuffer.get();
}


const VertexArray* Sprite::getVao() const
{
    return m_vao.get();
}


const IndexBuffer* Sprite::getIndexBuffer() const
{
    return m_indexBuffer.get();
}


const Shader* Sprite::getShader() const
{
    return m_shader.get();
}


// IDragDrop methods.
bool Sprite::IsDraggable() const
{
    return false;
}

bool Sprite::IsDropTarget() const
{
    return false;
}

bool Sprite::CanDrop(IDragDrop& item)
{
    return false;
}

void Sprite::DoDrop(IDragDrop& item)
{
}

void Sprite::SetTexture(std::shared_ptr<Texture> texture)
{
    m_texture = texture;
}
