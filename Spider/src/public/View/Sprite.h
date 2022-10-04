#pragma once

#include "DragDrop/IDragDrop.h"

#include <glm/glm.hpp>

#include <stdint.h>
#include <memory>


class Texture;
class VertexBuffer;
class VertexArray;
class IndexBuffer;
class Shader;
class SpriteFactory;
class CardModel;


enum class SpriteType { Card, Button, Slot };

class Sprite: public IDragDrop
{
public:
    Sprite(SpriteType type, SpriteFactory& factory);

    void LoadGraphics(
        const std::shared_ptr<Texture>& texture,
        const std::shared_ptr<VertexBuffer>& m_vertexBuffer,
        const std::shared_ptr<VertexArray>& m_vao,
        const std::shared_ptr<IndexBuffer>& m_indexBuffer,
        const std::shared_ptr<Shader>& m_shader,
        const glm::vec3& size);


    SpriteType getType() const;
    // position of the upper left corner.
    virtual void setPosition(const glm::vec2& position);
    virtual void setPosition(const glm::vec3& position);
    glm::vec2 getTopLeft() const;

    const glm::vec3& getPosition() const;
    const Texture* getTexture() const;

    virtual void setSize(const glm::vec2& size);
    const glm::vec3& getSize() const;

    void setAngleDegrees(float angle);
    float getAngleDegrees() const;
    void setAngleRadians(float angle);
    float getAngleRadians() const;

    const VertexBuffer* getVertexBuffer() const;
    const VertexArray* getVao() const;
    const IndexBuffer* getIndexBuffer() const;
    const Shader* getShader() const;


    // IDragDrop
    bool IsDraggable() const override;
    bool IsDropTarget() const override;
    bool CanDrop(IDragDrop& item) override;
    void DoDrop(IDragDrop& item) override;

protected:
    void SetTexture(std::shared_ptr<Texture> texture);


private:
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<Shader> m_shader;

    glm::vec3 m_position;
    glm::vec3 m_size;
    float m_angle;  // radians

protected:
    SpriteType m_type;
    SpriteFactory& m_factory;

};