#pragma once

#include "Sprite.h"

#include <array>
#include <memory>
#include <string>
#include <unordered_map>

class Texture;
class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class CardModel;
class GameResources;
class Card;
class Button;
class StackSlot;
class CardStack;
class SpriteRenderer;

class SpriteFactory
{
private:
    SpriteRenderer& m_spriteRenderer;
    GameResources& m_gameResources;
    std::array<std::shared_ptr<Texture>, 52> m_cardFaces;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_buttonMap;
    std::shared_ptr<Texture> m_stackSlot;
    std::shared_ptr<Texture> m_cardBack;

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

public:
    SpriteFactory(SpriteRenderer& spiderRenderer, GameResources& gameResources);
    ~SpriteFactory() = default;

    SpriteRenderer& getSpriteRenderer();

    void SetGraphics(Sprite& sprite, std::shared_ptr<Texture>& texture);

    std::shared_ptr<Button> CreateButton(const std::string& name);
    void SetGraphicsFromButtonName(const std::string& name, Button& button);

    std::shared_ptr<Card> CreateCard(const CardModel& cardModel);
    void SetGraphicsFromCardModel(const CardModel& cardModel, Card& card);

    std::shared_ptr<StackSlot> CreateStackSlot(CardStack& stack);
    void SetGraphicsForStackSlot(StackSlot& stackSpace);
};