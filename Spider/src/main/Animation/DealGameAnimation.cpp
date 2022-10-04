#include "Animation/DealGameAnimation.h"

#include "View/Card.h"
#include "View/CardStack.h"
#include "App/SpiderGame.h"
#include "Animation/SpiderAnimator.h"

#include <iostream>


DealGameAnimation::DealGameAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_doneCount(-1)
{
}


DealGameAnimation::~DealGameAnimation()
{
}

void DealGameAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}


void DealGameAnimation::Start(int delayMilliSeconds)
{
    int delay = delayMilliSeconds;

    m_doneCount = 10;

    for (int row = 0; row < 5; ++row)
    {
        for (int slot = 0; slot < 10; ++slot)
        {
            m_spiderAnimator.Empty([this, slot]() {this->StartOneDownCard(slot); }, delay);
            delay += 30;
        }
    }

    for (int slot = 0; slot < 4; ++slot)
    {
        m_spiderAnimator.Empty([this, slot]() {this->StartOneDownCard(slot); }, delay);
        delay += 30;
    }
}


void DealGameAnimation::DealOneCard(int slot, Animation::CompletionCB completionCB)
{
    Card& card = *m_game.getStock().RemoveTopCard();
    card.BringToTop();

    auto moveStack = std::make_unique<CardStack>(CardStack::Type::Drag);
    moveStack->setPosition(card.getPosition());
    moveStack->AddCard(card);

    m_spiderAnimator.SimpleStackMove(std::move(moveStack), m_game.getStack(slot), completionCB);
}


void DealGameAnimation::StartOneDownCard(int slot)
{
    DealOneCard(slot, [this, slot]() {this->DoneMoveDownCard(slot); });
}


void DealGameAnimation::DoneMoveDownCard(int slot)
{
    bool shouldTurnUp = false;
    CardStack& stack = m_game.getStack(slot);

    if (slot < 4)
        shouldTurnUp = stack.getCountOfCards() > 5;
    else
        shouldTurnUp = stack.getCountOfCards() > 4;

    if (!shouldTurnUp)
        return;

    m_spiderAnimator.TurnCard(stack.TopCard(), true, [this]() { this->DoneTurnCard(); });
}


void DealGameAnimation::DoneTurnCard()
{
    if (--m_doneCount > 0)
        return;

    BaseDone();
}
