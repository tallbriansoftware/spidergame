#pragma once

#include "DragDrop/IDragDrop.h"

#include <glm/glm.hpp>

#include <vector>

class Card;


class CardStack: public IDragDrop
{
public:
    enum class Type { Slot, Packs, Stock, Drag, Mock };
public:
    CardStack(Type type, int slotNumber = -1);

    void AddCard(Card& card);

    const std::vector<Card*>& getCards() const;
    int getCountOfCards() const;

    // Remove the cards from the given index or card reference
    // to the end of the list and return them in a vector.
    std::vector<Card*> RemoveCards(int index);
    std::vector<Card*> RemoveCards(Card& card);

    Card* RemoveTopCard();

    void setPosition(const glm::vec2& position);
    glm::vec2 getPosition() const;
    glm::vec2 getPositionOfNextCard() const;
    glm::vec2 getPositionOfCard(int cardNo, bool faceUp) const;

    glm::vec2 getSize() const;
    Type getStackType() const;
    int getSlotNumber() const;

    void setFaceUpSpacing(float spacing);
    void setFaceDownSpacing(float spacing);
    void setGapSpace(float gapSpace);
    void setGapSpacingInterval(int interval);

    void setIsDropTarget(bool value);

    // IDragDrop
    bool IsDraggable() const override;
    bool IsDropTarget() const override;
    bool CanDrop(IDragDrop& item) override;
    void DoDrop(IDragDrop& item) override;

    void DoDrop(std::vector<Card*> cards);

    bool IsLegalStackMove(const CardStack& stack) const;
    bool IsEmpty() const;
    Card& TopCard() const;
    bool HasFullPack() const;
    bool WillMakeAFullPack(CardStack& dropStack) const;
    bool IsCardUnderKingFaceDown() const;
    std::vector<Card*> RemoveFullPack();
    Card* GetMostDraggable();

private:
    void ReevaluateMovability();

private:
    Type m_stackType;
    int m_slotNumber;
    glm::vec2 m_position;
    std::vector<Card*> m_cards;
    bool m_isDropTarget;
    float m_faceUpSpacing;
    float m_faceDownSpacing;
    int m_gapSpacingInterval;
    float m_gapSpacing;
};