#include "App/TestConfigs.h"


namespace TestConfigs
{
    Tableau::Tableau(
        const std::array<std::vector<CardModel>, 10>& stacks,
        const std::vector<CardModel>& stock,
        const std::vector<CardModel>& packs)
    {
        m_stacks = stacks;
        m_stock = stock;
        m_packs = packs;
    }


    Tableau One(
        {
            std::vector<CardModel>(
            {   // Stack 1
                { Rank::Ten, Suit::Spades, false },
                { Rank::Ten, Suit::Hearts, false },
                { Rank::Eight, Suit::Hearts, false },
                { Rank::Four, Suit::Hearts, false },

                { Rank::Ace, Suit::Hearts, true},
                { Rank::King, Suit::Hearts, true },
                { Rank::Three, Suit::Hearts, true },
                { Rank::Two, Suit::Hearts, true },
                { Rank::Ace, Suit::Spades, true },
                { Rank::Four, Suit::Hearts, true },
                { Rank::Three, Suit::Hearts, true },
                { Rank::Two, Suit::Hearts, true },
                { Rank::Ace, Suit::Hearts, true },
            }),
            {   // stack 2
                { Rank::Six, Suit::Spades, false },
                { Rank::Nine, Suit::Hearts, false },
                { Rank::Five, Suit::Hearts, false },
                { Rank::Ace, Suit::Spades, false },
                { Rank::Eight, Suit::Hearts, false },

                { Rank::King, Suit::Hearts, true },
                { Rank::Queen, Suit::Hearts, true },
                { Rank::Jack, Suit::Hearts, true },
                { Rank::Ten, Suit::Hearts, true },
                { Rank::Nine, Suit::Hearts, true },
                { Rank::Eight, Suit::Hearts, true },
                { Rank::Seven, Suit::Hearts, true },
                { Rank::Six, Suit::Hearts, true },
            },
            {   // stack 3
                { Rank::Five, Suit::Spades, true },
                { Rank::Four, Suit::Spades, true },
                { Rank::Three, Suit::Spades, true },
            },
            {   // stack 4
                { Rank::Seven, Suit::Hearts, false },
                { Rank::Ten, Suit::Spades, false },
                { Rank::Two, Suit::Hearts, false },
                { Rank::Seven, Suit::Spades, false },

                { Rank::Queen, Suit::Hearts, true },
            },
            {   // stack 5
                { Rank::Five, Suit::Spades, false },
                { Rank::Three, Suit::Spades, false },
                { Rank::Jack, Suit::Spades, false },

                { Rank::Ace, Suit::Spades, true },
                { Rank::Five, Suit::Hearts, true },
                { Rank::Four, Suit::Spades, true },
                { Rank::Ace, Suit::Hearts, true },
                { Rank::Two, Suit::Spades, true },
                { Rank::Ace, Suit::Spades, true },

            },
            {   // stack 6
                { Rank::Jack, Suit::Spades, false },

                { Rank::King, Suit::Hearts, true },
                { Rank::Queen, Suit::Hearts, true },
                { Rank::Jack, Suit::Hearts, true },
                { Rank::Ten, Suit::Hearts, true },
                { Rank::Nine, Suit::Hearts, true },
                { Rank::Eight, Suit::Hearts, true },
                { Rank::Seven, Suit::Hearts, true },
            },
            {   // stack 7
                { Rank::Six, Suit::Hearts, false },
                { Rank::King, Suit::Spades, false },
                { Rank::Nine, Suit::Hearts, false },
                { Rank::Three, Suit::Hearts, false },

                { Rank::Five, Suit::Hearts, true },
                { Rank::Four, Suit::Hearts, true },
                { Rank::Three, Suit::Hearts, true },
                { Rank::Two, Suit::Hearts, true },
                { Rank::Ace, Suit::Hearts, true },
            },
            {   // stack 8
                { Rank::Three, Suit::Spades, false },
                { Rank::Four, Suit::Spades, false },

                { Rank::Six, Suit::Hearts, true },
                { Rank::Two, Suit::Spades, true },
                { Rank::King, Suit::Spades, true },
                { Rank::Queen, Suit::Spades, true },
                { Rank::Jack, Suit::Spades, true },
                { Rank::Ten, Suit::Spades, true },
                { Rank::Nine, Suit::Spades, true },
                { Rank::Eight, Suit::Spades, true },
                { Rank::Seven, Suit::Spades, true },
                { Rank::Six, Suit::Spades, true },
                { Rank::Five, Suit::Spades, true },
                { Rank::Nine, Suit::Spades, true },
                { Rank::Eight, Suit::Spades, true },
            },
            {   // stack 9
                { Rank::King, Suit::Spades, true },
            },
            {   // stack 5
                { Rank::Six, Suit::Spades, false },
                { Rank::Jack, Suit::Spades, false },
                { Rank::Four, Suit::Spades, false },

                { Rank::Jack, Suit::Hearts, true },
                { Rank::Ten, Suit::Hearts, true },
                { Rank::Nine, Suit::Spades, true },
                { Rank::Eight, Suit::Spades, true },
                { Rank::Seven, Suit::Spades, true },
            },

        },
        // Stock
        {
            { Rank::Five, Suit::Spades, false },
            { Rank::Six, Suit::Spades, false },
            { Rank::King, Suit::Spades, false },
            { Rank::Seven, Suit::Hearts, false },
            { Rank::Ten, Suit::Spades, false },
            { Rank::King, Suit::Hearts, false },
            { Rank::Seven, Suit::Spades, false },
            { Rank::Jack, Suit::Hearts, false },
            { Rank::Three, Suit::Spades, false },
            { Rank::Queen, Suit::Spades, false },
            { Rank::Two, Suit::Spades, false },
            { Rank::Eight, Suit::Spades, false },
            { Rank::Nine, Suit::Spades, false },
            { Rank::Four, Suit::Hearts, false },
            { Rank::Queen, Suit::Spades, false },
            { Rank::Five, Suit::Hearts, false },
            { Rank::Two, Suit::Spades, false },
            { Rank::Six, Suit::Hearts, false },
            { Rank::Queen, Suit::Spades, false },
            { Rank::Queen, Suit::Hearts, false },
        },
        // packs
        {
        });
}