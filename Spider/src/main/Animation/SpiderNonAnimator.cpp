#include "Animation/SpiderNonAnimator.h"

#include "View/Card.h"
#include "View/CardStack.h"
#include "App/SpiderGame.h"


SpiderNonAnimator::SpiderNonAnimator()
{
}


SpiderNonAnimator::~SpiderNonAnimator()
{
}



void SpiderNonAnimator::DealGame(SpiderGame& game)
{
    for (int row = 0; row < 5; ++row)
    {
        for (int slot = 0; slot < 10; ++slot)
        {
            DealOneCard(game, slot, false);
        }
    }
    for (int slot = 0; slot < 4; ++slot)
    {
        DealOneCard(game, slot, false);
    }
    for (int slot = 0; slot < 10; ++slot)
    {
        DealOneCard(game, slot, true);
    }
}


void SpiderNonAnimator::DealOneCard(SpiderGame& game, int slot, bool faceUp)
{
    Card& card = *game.getStock().RemoveTopCard();
    card.BringToTop();
    CardStack& stack = game.getStack(slot);
    if (faceUp)
        card.TurnFaceUp();
    else
        card.TurnFaceDown();
    stack.AddCard(card);
    card.BringToTop();
}
