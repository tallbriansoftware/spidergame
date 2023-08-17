#include "View/Card.h"

#include "View/Sprite.h"
#include "View/SpriteFactory.h"
#include "View/SpriteRenderer.h"
#include "View/CardStack.h"

#include <MyGL/GLErrors.h>


Card::Card(SpriteFactory* factory, const CardModel& cardModel)
    : Sprite(SpriteType::Card, *factory)
    , m_cardModel(cardModel)
    , m_isMovable(false)
    , m_isDropTarget(false)
    , m_parent(nullptr)
{
    m_factory.SetGraphicsFromCardModel(m_cardModel, *this);
}

void Card::setParentStack(CardStack* cardStack)
{
    assert(m_parent == nullptr);
    m_parent = cardStack;
}

CardStack* Card::getParentStack()
{
    return m_parent;
}

void Card::ClearParent()
{
    m_parent = nullptr;
}


void Card::setIsDropTarget(bool value)
{
    m_isDropTarget = value;
}


Rank Card::getRank() const
{
    return m_cardModel.getRank();
}


Suit Card::getSuit() const
{
    return m_cardModel.getSuit();
}


bool Card::IsFaceUp() const
{
    return m_cardModel.IsFaceUp();
}

void Card::TurnFaceUp()
{
    SetTexture(m_front);
    setMovable();
    m_cardModel.TurnFaceUp();

    if (m_parent != nullptr)
        m_parent->ReevaluateMovability();
}

void Card::TurnFaceDown()
{
    SetTexture(m_back);
    setMovable(false);
    m_cardModel.TurnFaceDown();

    if(m_parent != nullptr)
        m_parent->ReevaluateMovability();
}

void Card::setMovable(bool value)
{
    m_isMovable = value;
}

void Card::SetCardTextures(std::shared_ptr<Texture> front, std::shared_ptr<Texture> back)
{
    m_front = front;
    m_back = back;
}

void Card::BringToTop()
{
    m_factory.getSpriteRenderer().BringToTop(*this);
}


// ===== IDragDrop ==========
//
bool Card::IsDraggable() const
{
    if(m_parent->getStackType() == CardStack::Type::Slot)
        return m_isMovable;
    return false;
}

// All cards are part of a stack
// drop on a card is to drop on the stack.
bool Card::IsDropTarget() const
{
    return m_parent->IsDropTarget();
}

bool Card::CanDrop(IDragDrop& item)
{
    if (!m_parent->IsDropTarget())
        return false;

    return m_parent->CanDrop(item);
}

void Card::DoDrop(IDragDrop& item)
{
    m_parent->DoDrop(item);
}
