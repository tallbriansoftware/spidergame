#pragma once

#include "Model/CardEnums.h"

class CardModel
{
public:
    CardModel();
    CardModel(Rank rank, Suit suit, bool isFaceUp=false);

    CardModel(const CardModel& card);
    CardModel& operator=(const CardModel& card);

    ~CardModel();
    bool operator==(const CardModel& rhs) const;
    bool operator!=(const CardModel& rhs) const;


    Rank getRank() const;
    Suit getSuit() const;

    int getSortValue() const;

    bool IsFaceUp() const;
    CardModel& TurnFaceUp();
    CardModel& TurnFaceDown();

#ifdef _DEBUG
    const char* getName();
#endif


private:
    void AssertCardIsValid() const;
    void AssertCardIsFaceUp() const;

#ifdef _DEBUG
    void setName();
#endif

private:
    char m_cardValue;
#ifdef _DEBUG
    char m_name[3];
#endif
};
