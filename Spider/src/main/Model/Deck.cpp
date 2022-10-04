#include "Model/Deck.h"

#include "Model/CardModel.h"

#include <vector>
#include <cassert>


std::vector<CardModel> Deck::Create(int numSuits)
{
    std::vector<Suit> suitVec;
    switch (numSuits)
    {
    case 1:
        suitVec = { Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades, };
        break;

    case 2:
        suitVec = { Suit::Spades, Suit::Hearts, Suit::Spades, Suit::Hearts, Suit::Spades, Suit::Hearts, Suit::Spades, Suit::Hearts, };
        break;

    case 4:
        suitVec = { Suit::Spades, Suit::Hearts, Suit::Diamonds, Suit::Clubs, Suit::Spades, Suit::Hearts, Suit::Diamonds, Suit::Clubs };
        break;

    default:
        assert("Invalid number of Suits");
        break;
    }

    std::vector<Rank> rankVec = { Rank::Ace, Rank::Two, Rank::Three, Rank::Four, Rank::Five, Rank::Six,
        Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King };

    std::vector<CardModel> deck;

    for (Suit suit : suitVec)
    {
        for (Rank rank : rankVec)
        {
            CardModel card(rank, suit);
            card.TurnFaceUp();
            deck.push_back(card);
        }
    }
    return deck;
}
