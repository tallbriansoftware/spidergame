#pragma once

#include "View/Sprite.h"

class CardStack;

class StackSlot: public Sprite
{
public:
    StackSlot(SpriteFactory* factory, CardStack& cardStack);
    void LoadImage();

    // from Sprite
    virtual void setPosition(const glm::vec2& position);
    virtual void setPosition(const glm::vec3& position);


    void setSlotNumber(int num);
    int getSlotNumner();

    CardStack& getCardStack();

    // IDragDrop
    bool IsDraggable() const override;
    bool IsDropTarget() const override;
    bool CanDrop(IDragDrop& item) override;
    void DoDrop(IDragDrop& item) override;

private:
    int m_slotNumber;
    CardStack& m_cardStack;
};