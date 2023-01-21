#include "View/SpriteFactory.h"

#include "App/GameResources.h"
#include "MyGL/IndexBuffer.h"
#include "MyGL/Texture.h"
#include "MyGL/Shader.h"
#include "MyGL/VertexArray.h"
#include "MyGL/VertexBuffer.h"
#include "MyGL/VertexBufferLayout.h"
#include "Model/CardModel.h"
#include "View/Card.h"
#include "View/Sprite.h"
#include "View/Button.h"
#include "View/StackSlot.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace
{
    float cardVertices[] = {
        // position      /  texture offset
         -0.5f, -0.5f,     0.0f, 0.0f,     // 0
          0.5f, -0.5f,     1.0f, 0.0f,     // 1
          0.5f,  0.5f,     1.0f, 1.0f,     // 2
         -0.5f,  0.5f,     0.0f, 1.0f,     // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    void LoadCardTextures(std::array<std::shared_ptr<Texture>, 52>& cardFaces, const std::string& cardPath)
    {
        int i = 0;
        // this must be the same order as the enum in CardEnums.h
        for (std::string suit : { "Spades", "Hearts", "Diamonds", "Clubs" })
        {
            // this must be the same order as the enum in CardEnums.h
            for (std::string rank : { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
                "Eight", "Nine", "Ten", "Jack", "Queen", "King"})
            {
                std::string filepath = cardPath + "/Card-" + rank + "-Of-" + suit + ".png";
                cardFaces[i++] = std::make_shared<Texture>(filepath);
            }
        }
    }

    void LoadButtonTextures(std::unordered_map<std::string, std::shared_ptr<Texture>>& textureMap, const std::string& buttonPath)
    {
        std::string buttonNames[] = { "Hint", "Undo", "Test1", "NewGame"};

        for (auto name : buttonNames)
        {
            std::string filepath = buttonPath + "/" + name + ".png";
            auto texture = std::make_shared<Texture>(filepath);
            textureMap[name] = texture;
        }
    }

    void LoadStackSlotTexture(std::shared_ptr<Texture>& texture, const std::string& cardPath)
    {
        std::string filepath = cardPath + "/Slot.png";
        texture = std::make_shared<Texture>(filepath);
    }

    void LoadCardBackTexture(std::shared_ptr<Texture>& texture, const std::string& cardPath)
    {
        std::string filepath = cardPath + "/Back.png";
        texture = std::make_shared<Texture>(filepath);
    }
}


SpriteFactory::SpriteFactory(SpriteRenderer& spriteRenderer, GameResources& gameResources)
    : m_spriteRenderer(spriteRenderer)
    , m_gameResources(gameResources)
{
    // load all the textures
    LoadCardTextures(m_cardFaces, m_gameResources.GetCardPath());
    LoadButtonTextures(m_buttonMap, m_gameResources.GetButtonPath());
    LoadStackSlotTexture(m_stackSlot, m_gameResources.GetCardPath());
    LoadCardBackTexture(m_cardBack, m_gameResources.GetCardPath());

    // Load the shader
    std::string shaderPath = m_gameResources.GetShaderPath() + "/Sprite.shader";
    m_shader = std::make_shared<Shader>(shaderPath);

    // Load the Vertex Buffer
    m_vertexBuffer = std::make_shared<VertexBuffer>(cardVertices, sizeof(cardVertices));

    // Load the "types" in the Vertex Buffer
    VertexBufferLayout layout;
    layout.Push<float>(2);  // two floats of position.
    layout.Push<float>(2);  // two floats of texture offset.

    // Load the VertexBuffer with Layout Types into a VertexArray
    m_vao = std::make_unique<VertexArray>();
    m_vao->AddBuffer(*m_vertexBuffer, layout);

    // Load the IndexBuffer
    m_indexBuffer = std::make_unique<IndexBuffer>(indices, 6);
}


SpriteRenderer& SpriteFactory::getSpriteRenderer()
{
    return m_spriteRenderer;
}

void SpriteFactory::SetGraphics(Sprite& sprite, std::shared_ptr<Texture>& texture)
{
    glm::vec3 size((float)texture->GetWidth(), (float)texture->GetHeight(), 0.0f);
    sprite.LoadGraphics(texture, m_vertexBuffer, m_vao, m_indexBuffer, m_shader, size);
}


// =========== Cards =============

std::shared_ptr<Card> SpriteFactory::CreateCard(const CardModel& cardModel)
{
    auto card = std::make_shared<Card>(this, cardModel);
    return card;
}

void SpriteFactory::SetGraphicsFromCardModel(const CardModel& cardModel, Card& card)
{
    int cardNo = (int)cardModel.getSuit() * 13 + (int)cardModel.getRank();
    std::shared_ptr<Texture> texture = m_cardFaces[cardNo];
    SetGraphics(card, texture);
    card.SetCardTextures(texture, m_cardBack);
}

// ========= Stack Spaces ====================

std::shared_ptr<StackSlot> SpriteFactory::CreateStackSlot(CardStack& stack)
{
    auto slot = std::make_shared<StackSlot>(this, stack);
    slot->LoadImage();
    return slot;
}

void SpriteFactory::SetGraphicsForStackSlot(StackSlot& slot)
{
    std::shared_ptr<Texture> texture = m_stackSlot;
    SetGraphics(slot, texture);
}

// =========== Buttons ======================

std::shared_ptr<Button> SpriteFactory::CreateButton(const std::string& name)
{
    auto button = std::make_shared<Button>(this);
    button->LoadImage(name);
    return button;
}

void SpriteFactory::SetGraphicsFromButtonName(const std::string& name, Button& button)
{
    auto texture = m_buttonMap.at(name);
    SetGraphics(button, texture);
}
