#include "View/CardStack.h"

#include "View/Card.h"

#include <algorithm>
#include <vector>
#include <MyGL/GLErrors.h>


CardStack::CardStack(CardStack::Type type, int slotNumber)
    : m_stackType(type)
    , m_slotNumber(slotNumber)
    , m_position(0.0f, 0.0f)
    , m_isDropTarget(false)
    , m_faceUpSpacing(60.0f)
    , m_faceDownSpacing(20.0f)
    , m_gapSpacingInterval(0)
    , m_gapSpacing(0.0f)
{
}


void CardStack::AddCard(Card& card)
{
    glm::vec2 position = getPositionOfCard(getCountOfCards(), card.IsFaceUp());
    card.setPosition(position);

    m_cards.push_back(&card);
    card.setParentStack(this);
    ReevaluateMovability();
}


const std::vector<Card*>& CardStack::getCards() const
{
    return m_cards;
}


int CardStack::getCountOfCards() const
{
    return (int)m_cards.size();
}


namespace
{
    std::vector<Card*> RemoveCards_itr(
        std::vector<Card*>& vec,
        std::vector<Card*>::iterator& chosen)
    {
        // Copy the elements into the return value;
        std::vector<Card*> cards(chosen, vec.end());

        // Erase them from the original list;
        vec.erase(chosen, vec.end());

        // Release the parent, so they can be reassigned.
        for (auto card : cards)
            card->ClearParent();

        return cards;;
    }
}

std::vector<Card*> CardStack::RemoveCards(int index)
{
    auto start = m_cards.begin() + index;
    auto removedCards = RemoveCards_itr(m_cards, start);
    ReevaluateMovability();

    return removedCards;
}

std::vector<Card*> CardStack::RemoveCards(Card& chosen)
{
    auto start = std::find(m_cards.begin(), m_cards.end(), &chosen);
    if (start == m_cards.end())
        return {};  // should I throw an error?

    auto removedCards = RemoveCards_itr(m_cards, start);
    ReevaluateMovability();

    return removedCards;
}

Card* CardStack::RemoveTopCard(bool bringToTop)
{
    auto count = m_cards.size();
    if (count == 0)
        return nullptr;

    Card* card = m_cards[count - 1];

    // remove card from original parent.
    auto lastCard = m_cards.end() - 1;
    m_cards.erase(lastCard, m_cards.end());

    card->ClearParent();
    if(bringToTop)
        card->BringToTop();

    ReevaluateMovability();

    return card;
}


void CardStack::setPosition(const glm::vec2& position)
{
    m_position = position;
    for(int i=0; i<(int)m_cards.size(); i++)
    {
        auto& card = m_cards[i];
        glm::vec2 cardPos(m_position.x, m_position.y + i * m_faceUpSpacing);
        card->setPosition(cardPos);
    }
}


glm::vec2 CardStack::getPosition() const
{
    return m_position;
}


// get the position of the next card that will be added to the stack.
glm::vec2 CardStack::getPositionOfNextCard() const
{
    return getPositionOfCard(getCountOfCards() + 1, true);
}

glm::vec2 CardStack::getPositionOfCard(int cardNo, bool faceUp) const
{
    // Assumption:   A stack of cards is:
    //  zero or more face down cards
    // followed by zero or more face up cards.

    if (cardNo < getCountOfCards())
        return m_cards[cardNo]->getPosition();

    int numExtraCards = (cardNo - getCountOfCards()) + 1;
    auto position = IsEmpty() ? m_position : TopCard().getPosition();

    float delta = (numExtraCards - 1) * (faceUp ? m_faceUpSpacing : m_faceDownSpacing);
    delta += IsEmpty() ? 0 : (TopCard().IsFaceUp() ? m_faceUpSpacing : m_faceDownSpacing);

    int gaps = 0;
    if (m_gapSpacingInterval != 0)
        gaps = ((cardNo) / m_gapSpacingInterval) - ((getCountOfCards() - 1) / m_gapSpacingInterval);
    delta += gaps * m_gapSpacing;

    return { position.x, position.y + delta };
}


glm::vec2 CardStack::getSize() const
{
    if (m_cards.size() == 0)
        return glm::vec2(0.0f, 0.0f);

    auto& lastCard = m_cards[m_cards.size() - 1];
    glm::vec2 bottomRight(lastCard->getPosition() + lastCard->getSize());
    glm::vec2 size(bottomRight - m_position);
    return size;
}

CardStack::Type CardStack::getStackType() const
{
    return m_stackType;
}

int CardStack::getSlotNumber() const
{
    return m_slotNumber;
}

void CardStack::setFaceUpSpacing(float spacing)
{
    m_faceUpSpacing = spacing;
}

void CardStack::setFaceDownSpacing(float spacing)
{
    m_faceDownSpacing = spacing;
}

void CardStack::setGapSpace(float gapSpace)
{
    m_gapSpacing = gapSpace;
}

void CardStack::setGapSpacingInterval(int interval)
{
    m_gapSpacingInterval = interval;
}

// =======  DragDrop  =================

bool CardStack::IsDraggable() const
{
    return false;
}

void CardStack::setIsDropTarget(bool value)
{
    m_isDropTarget = value;
}

bool CardStack::IsDropTarget() const
{
    return m_isDropTarget;
}

bool CardStack::CanDrop(IDragDrop& item)
{
    if (!m_isDropTarget)
        return false;

    CardStack* stack = dynamic_cast<CardStack*>(&item);
    if (stack == nullptr)
        return false;

    if (!TopCard().IsFaceUp())
        return false;

    return IsLegalStackMove(*stack);
}

void CardStack::DoDrop(IDragDrop& item)
{
    CardStack* stack = dynamic_cast<CardStack*>(&item);
    if (stack == nullptr)
    {
        LOG_ERROR("item dropped on a stack is not a stack");
        return;
    }

    std::vector<Card*> cards = stack->RemoveCards(0);
    DoDrop(cards);
}

void CardStack::DoDrop(std::vector<Card*> cards)
{
    for (auto card : cards)
        this->AddCard(*card);
}

bool CardStack::IsLegalStackMove(const CardStack& stack) const
{
    if (m_cards.size() == 0)
        return true;

    Card& showingCard = *m_cards[m_cards.size() - 1];
    Card& topDragCard = *stack.m_cards[0];
    return (int)showingCard.getRank() == ((int)topDragCard.getRank() + 1);
}

bool CardStack::IsEmpty() const
{
    return m_cards.size() == 0;
}

Card& CardStack::TopCard() const
{
    // caller is responsible for calling !IsEmpty() before calling this.
    int topIndex = (int)(m_cards.size()) - 1;
    return *m_cards[topIndex];
}

bool CardStack::HasFullPack() const
{
    size_t count = m_cards.size();
    if (count < 13)
        return false;

    if (TopCard().getRank() != Rank::Ace)
        return false;

    Suit suit = TopCard().getSuit();

    for (int i = 1; i < 13; i++)
    {
        Card& card = *m_cards[count - 1 - i];
        if (!card.IsFaceUp())
            return false;
        if (card.getSuit() != suit)
            return false;
        if (card.getRank() != (Rank)i)
            return false;
    }
    return true;
}


// If the dropstack is dropped on this stack
// Will it make a full suited pack?
bool CardStack::WillMakeAFullPack(CardStack& dropStack) const
{
    size_t count = m_cards.size() + dropStack.m_cards.size();
    if (count < 13)
        return false;

    int i_rank = (int)Rank::Ace;
    Suit suit = dropStack.TopCard().getSuit();

    for (size_t drop_i = 0; drop_i < dropStack.m_cards.size(); ++drop_i)
    {
        // Count backwards through the "dropStack" cards.
        Card* card = dropStack.m_cards[dropStack.m_cards.size() - 1 - drop_i];
        if (card->getRank() != (Rank)i_rank++)
            return false;
        if (card->getSuit() != suit)
            return false;
    }

    for (size_t target_i = 0; target_i < m_cards.size(); ++target_i)
    {
        // Count backwards through "this" the cards.
        Card* card = m_cards[m_cards.size() - 1 - target_i];
        if (!card->IsFaceUp())
            return false;
        if (card->getRank() != (Rank)i_rank++)
            return false;
        if (card->getSuit() != suit)
            return false;
        if (i_rank > (int)Rank::King)
            break;
    }
    return i_rank > (int)Rank::King;
}


// when clearing a pack will we need to turn a card under it?
bool CardStack::IsCardUnderKingFaceDown() const
{
    if (m_cards.empty())
        return false;

    if (m_cards[0]->IsFaceUp())
        return false;

    for (Card* card : m_cards)
    {
        if (card->IsFaceUp())
        {
            if (card->getRank() == Rank::King)
                return true;
            return false;
        }
    }
    // should never get here.  That would be
    // a pile of all facedown cards.
    return false;
}


std::vector<Card*> CardStack::RemoveFullPack()
{
    int count = (int)m_cards.size();
    std::vector<Card*> donePack = RemoveCards(count - 13);
    ReevaluateMovability();
    return donePack;
}

Card* CardStack::GetMostDraggable()
{
    for (auto card : m_cards)
    {
        if (card->IsDraggable())
            return card;
    }
    return nullptr;
}

void CardStack::ReevaluateMovability()
{
    if (getStackType() != Type::Slot)
        return;

    if (IsEmpty())
        return;

    if (!TopCard().IsFaceUp())
        return;

    TopCard().setMovable();
    
    int count = (int)m_cards.size();
    if (count < 2)
            return;

    Suit suit = TopCard().getSuit();
    Rank rank = TopCard().getRank();

    int index;
    for (index = count-2; index >= 0;  --index)
    {
        Card& card = *m_cards[index];
        if (!card.IsFaceUp())
            break;

        if(rank == Rank::King)
            break;
        rank = (Rank)((int)rank + 1);

        if (card.getSuit() != suit)
            break;
        if (card.getRank() != rank)
            break;

        card.setMovable();
    }
    for (; index >= 0; --index)
    {
        m_cards[index]->setMovable(false);
    }
}
