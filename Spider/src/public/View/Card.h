#pragma once

#include "Model/CardModel.h"
#include "View/Sprite.h"

#include "glm/glm.hpp"

#include <memory>
#include <string>

class CardStack;
class SpriteFactory;


class Card: public Sprite
{
public:
    Card(SpriteFactory* factory, const CardModel& cardModel);

    void setParentStack(CardStack* cardStack);
    CardStack* getParentStack();
    void ClearParent();

    void setIsDropTarget(bool value=true);

    Rank getRank() const;
    Suit getSuit() const;
    bool IsFaceUp() const;

    void TurnFaceUp();
    void TurnFaceDown();
    void setMovable(bool value = true);

    void SetCardTextures(std::shared_ptr<Texture> front, std::shared_ptr<Texture> back);
    void BringToTop();

    
    // IDragDrop
    bool IsDraggable() const override;
    bool IsDropTarget() const override;
    bool CanDrop(IDragDrop& item) override;
    void DoDrop(IDragDrop& item) override;

private:
    CardModel m_cardModel;
    bool m_isMovable;
    bool m_isDropTarget;
    CardStack* m_parent;
    std::shared_ptr<Texture> m_front;
    std::shared_ptr<Texture> m_back;
};