#include "Model/CardModel.h"
//#include "AutoPlayCore/Core/SpiderException.h"

#include <stdint.h>

#include "assert.h"

namespace
{
    const Rank s_valueToRank[64] = {
        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,
    };

    const Suit s_valueToSuit[64] = {
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::NaS, Suit::NaS, Suit::NaS,
    };

    const int ValueMask = 0x3F;
    const int FaceUpBit = 0x40;

    uint8_t RankAndSuitToValue(Rank rank, Suit suit)
    {
        int suitPart = static_cast<int>(suit) * 16;
        int rankPart = static_cast<int>(rank);
        return suitPart + rankPart;
    }
}


CardModel::CardModel()
    : m_cardValue(-1)
{
#ifdef _DEBUG
    setName();
#endif
}


CardModel::CardModel(Rank rank, Suit suit, bool isFaceUp)
{
    int suitPart = static_cast<int>(suit) * 16;
    int rankPart = static_cast<int>(rank);
    m_cardValue = suitPart + rankPart;
    m_cardValue |= isFaceUp ? FaceUpBit : 0;

#ifdef _DEBUG
    setName();
#endif
}


CardModel::CardModel(const CardModel& card)
    : m_cardValue(card.m_cardValue)

{
#ifdef _DEBUG
    setName();
#endif
}

CardModel& CardModel::operator=(const CardModel& card)
{
    m_cardValue = card.m_cardValue;
#ifdef _DEBUG
    setName();
#endif
    return *this;
}


CardModel::~CardModel() = default;

#ifdef _DEBUG
const char* CardModel::getName()
{
    return m_name;
}
#endif


void CardModel::AssertCardIsValid() const
{
    assert(m_cardValue >= 0);

    Rank rank = s_valueToRank[m_cardValue & ValueMask];
    Suit suit = s_valueToSuit[m_cardValue & ValueMask];

    assert(rank != Rank::NaR && suit != Suit::NaS);
}


void CardModel::AssertCardIsFaceUp() const
{
    assert(m_cardValue & FaceUpBit);
}


bool CardModel::operator==(const CardModel & rhs) const
{
#ifdef _DEBUG
    AssertCardIsValid();
    rhs.AssertCardIsValid();
#endif
    return (m_cardValue & ValueMask) == (rhs.m_cardValue & ValueMask);
}


bool CardModel::operator!=(const CardModel & rhs) const
{
    return !(*this == rhs);
}


Rank CardModel::getRank() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return s_valueToRank[m_cardValue & ValueMask];
}


int CardModel::getSortValue() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return m_cardValue & ValueMask;
}


Suit CardModel::getSuit() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return s_valueToSuit[m_cardValue & ValueMask];
}


bool CardModel::IsFaceUp() const
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    return m_cardValue & FaceUpBit;
}


CardModel& CardModel::TurnFaceUp()
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    m_cardValue |= FaceUpBit;
    return *this;
}


CardModel& CardModel::TurnFaceDown()
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    m_cardValue &= ValueMask;
    return *this;
}


#ifdef _DEBUG
void CardModel::setName()
{
    static char Suits[] = "SHDC";
    static char Ranks[] = "A23456789TJQK";
    Suit suitPart = s_valueToSuit[m_cardValue & ValueMask];
    Rank rankPart = s_valueToRank[m_cardValue & ValueMask];
    m_name[0] = Ranks[static_cast<int>(rankPart)];
    m_name[1] = Suits[static_cast<int>(suitPart)];
    m_name[2] = 0;
}
#endif
