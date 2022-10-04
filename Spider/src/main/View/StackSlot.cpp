#include "View/StackSlot.h"

#include "View/SpriteFactory.h"
#include "View/Sprite.h"
#include "View/Card.h"
#include "View/CardStack.h"

#include <MyGL/GLErrors.h>


StackSlot::StackSlot(SpriteFactory* factory, CardStack& cardStack)
    : Sprite(SpriteType::Slot, *factory)
    , m_slotNumber(-1)
    , m_cardStack(cardStack)
{
}

void StackSlot::LoadImage()
{
    m_factory.SetGraphicsForStackSlot(*this);
}

void StackSlot::setPosition(const glm::vec2& position)
{
    Sprite::setPosition(position);
    m_cardStack.setPosition(position);
}

void StackSlot::setPosition(const glm::vec3& position)
{
    Sprite::setPosition(position);
    m_cardStack.setPosition({ position.x, position.y });
}

void StackSlot::setSlotNumber(int num)
{
    m_slotNumber = num;
}

int StackSlot::getSlotNumner()
{
    return m_slotNumber;
}


CardStack& StackSlot::getCardStack()
{
    return m_cardStack;
}


// ====  IDragDrop ===================
//
bool StackSlot::IsDraggable() const
{
    return false;
}

bool StackSlot::IsDropTarget() const
{
    return true;
}

bool StackSlot::CanDrop(IDragDrop& item)
{
    return true;
}

void StackSlot::DoDrop(IDragDrop& item)
{
    CardStack* stack = dynamic_cast<CardStack*>(&item);
    if (stack == nullptr)
    {
        LOG_ERROR("Item Droped on a Stack Space is not a Stack");
        return;
    }

    std::vector<Card*> cards = stack->RemoveCards(0);
    for (auto card : cards)
        m_cardStack.AddCard(*card);
}
