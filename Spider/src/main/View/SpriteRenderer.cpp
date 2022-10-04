#include "View/SpriteRenderer.h"

#include "View/Sprite.h"
#include "View/Card.h"
#include "View/Button.h"
#include "View/StackSlot.h"
#include "View/CardStack.h"

#include "MyGL/Platform/Window2D.h"
#include "MyGL/Shader.h"
#include "MyGL/Texture.h"
#include "MyGL/VertexArray.h"
#include "MyGL/VertexBuffer.h"
#include "MyGL/IndexBuffer.h"

#include <MyGL/GLErrors.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GL/glew.h>

#include <algorithm>


namespace
{
    void EraseRemove(std::vector<Sprite*> vec, Sprite& value)
    {
        vec.erase(std::remove(
            vec.begin(), vec.end(), &value),
            vec.end());
    }

    void EraseRemove(std::vector<std::shared_ptr<Sprite>> vec, Sprite& value)
    {
        vec.erase(std::remove_if(
            vec.begin(), vec.end(),
                [&value](const std::shared_ptr<Sprite>& item) { return item.get() == &value; }),
            vec.end());
    }

    void DrawSprite(const Sprite& sprite, const glm::mat4& viewProjection)
    {
        sprite.getVao()->Bind();
        sprite.getTexture()->Bind();

        // Position is externally upper left corner
        glm::vec3 position = sprite.getPosition();
        glm::vec3 size = sprite.getSize();
        glm::vec3 center = position + (size / 2.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, center);
        model = glm::scale(model, size);
        model = glm::rotate(model, sprite.getAngleRadians(), glm::vec3(0, 1, 0));
        glm::mat4 mvp = viewProjection * model;

        auto shader = sprite.getShader();
        shader->Bind();
        shader->SetUniformMat4f("u_MVP", mvp);

        auto indexBuffer = sprite.getIndexBuffer();
        indexBuffer->Bind();

        GLCall(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
    }
}

void SpriteRenderer::AddButton(const std::shared_ptr<Sprite>& button)
{
    m_spriteHolder.push_back(button);
    m_buttonDrawList.push_back(button.get());
}

void SpriteRenderer::RemoveButton(Sprite& button)
{
    EraseRemove(m_buttonDrawList, button);
    EraseRemove(m_spriteHolder, button);
}


void SpriteRenderer::AddCard(const std::shared_ptr<Sprite>& card)
{
    m_spriteHolder.push_back(card);
    m_cardDrawList.push_back(card.get());
}

void SpriteRenderer::RemoveCard(Sprite& card)
{
    EraseRemove(m_cardDrawList, card);
    EraseRemove(m_spriteHolder, card);
}

void SpriteRenderer::AddStackSlot(const std::shared_ptr<StackSlot>& slot)
{
    m_spriteHolder.push_back(slot);
    m_slotDrawList.push_back(slot.get());
}

void SpriteRenderer::RemoveStackSlot(StackSlot& slot)
{
    EraseRemove(m_slotDrawList, slot);
    EraseRemove(m_spriteHolder, slot);
}


void SpriteRenderer::DrawSprites(const MyGL::Window2D& window2D) const
{
    glm::mat4 viewProjection = window2D.GetViewProjection();

    for (auto sprite : m_slotDrawList)
    {
        DrawSprite(*sprite, viewProjection);
    }

    for (auto sprite : m_buttonDrawList)
    {
        DrawSprite(*sprite, viewProjection);
    }

    for (auto sprite : m_cardDrawList)
    {
        DrawSprite(*sprite, viewProjection);
    }
}


namespace
{
    Sprite* HitTest_OnList(
        std::vector<Sprite*> vec,
        const MyGL::Window2D& window2D,
        const glm::vec2& testPos,
        const std::vector<Card*>* excludeList = nullptr)
    {
        int size = (int)vec.size();
        for (int i = size - 1; i >= 0; i--)
        {
            Sprite* sprite = vec[i];

            // Skip possibly the sprite that is testing.
            if (excludeList != nullptr)
            {
                if (std::find(excludeList->begin(), excludeList->end(), sprite) != excludeList->end())
                    continue;
            }

            glm::vec3 topLeft = sprite->getPosition();
            glm::vec3 bottomRight = topLeft + sprite->getSize();;

            if (testPos.x >= topLeft.x && testPos.x < bottomRight.x)
            {
                if (testPos.y >= topLeft.y && testPos.y < bottomRight.y)
                {
                    return sprite;
                }
            }
        }

        return nullptr;
    }
}


Sprite* SpriteRenderer::HitTest(const MyGL::Window2D& window2D, const glm::vec2& testPos, const std::vector<Card*>* excludeList) const
{
    Sprite* sprite = nullptr;

    sprite = HitTest_OnList(m_cardDrawList, window2D, testPos, excludeList);
    if (sprite != nullptr)
        return sprite;

    sprite = HitTest_OnList(m_slotDrawList, window2D, testPos, excludeList);
    if (sprite != nullptr)
        return sprite;

    sprite = HitTest_OnList(m_buttonDrawList, window2D, testPos, excludeList);
    if (sprite != nullptr)
        return sprite;

    return nullptr;
}


Sprite* SpriteRenderer::HitTest(const MyGL::Window2D& window2D, const CardStack& dropStack) const
{
    glm::vec2 size = dropStack.getSize();
    glm::vec2 offsets[] = {
        {size.x / 2.0f, 0.0f},  // Middle top edge
        {0.0f, 0.0f},           // top left corner
        {size.x, 0.0f},         // top right corner
        {size.x / 2.0f, size.y / 2.0f}, // center of the sprite
    };

    glm::vec2 topLeft = dropStack.getPosition();
    const std::vector<Card*>& excludeList = dropStack.getCards();

    for (auto& offset : offsets)
    {
        auto testPos = topLeft + offset;
        Sprite* sprite = HitTest(window2D, testPos, &excludeList);
        if (sprite != nullptr)
            return sprite;
    }

    return nullptr;
}

void SpriteRenderer::BringToTop(Sprite& sprite)
{
    auto found_itr = find(m_cardDrawList.begin(), m_cardDrawList.end(), &sprite);
    m_cardDrawList.erase(found_itr);
    m_cardDrawList.push_back(&sprite);
}