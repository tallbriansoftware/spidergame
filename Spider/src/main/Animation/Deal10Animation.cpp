#include "Animation/Deal10Animation.h"

#include "View/Card.h"
#include "View/CardStack.h"
#include "App/SpiderGame.h"
#include "Animation/SpiderAnimator.h"
    
#include <iostream>

namespace
{
    const int ms_betweenCards = 30;
    const int ms_delayBeforeRemovingFullStack = 500;
}

Deal10Animation::Deal10Animation(SpiderAnimator& spiderAnimator, SpiderGame& game)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_doneCount(-1)
{
}


Deal10Animation::~Deal10Animation()
{
}

void Deal10Animation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    assert("cannot set this callback on this animation" == nullptr);
}

void Deal10Animation::SetCompletionCallback(Deal10Animation::CompletionCB doneCB)
{
    m_dealMoveCompletionCB = doneCB;
}

void Deal10Animation::Start(int delayMilliSeconds)
{
    m_doneCount = 10;

    m_dealStack = std::make_unique<CardStack>(CardStack::Type::Mock);
    m_dealStack->setFaceDownSpacing(2.0f);
    m_dealStack->setGapSpacingInterval(10);
    m_dealStack->setGapSpace(16.0f);

    int index = m_game.getStock().getCountOfCards() - 10;
    std::vector<Card*> cardVec = m_game.getStock().RemoveCards(index);
    m_dealStack->setPosition(cardVec[0]->getPosition());

    for (Card* card : cardVec)
    {
        card->BringToTop();
        m_dealStack->AddCard(*card);
    }

    for (int slot = 0; slot < 10; ++slot)
    {
        int delay = delayMilliSeconds + slot * ms_betweenCards;
        m_spiderAnimator.Empty([this, slot]() { this->StartCard(slot); }, delay);
    }
}


void Deal10Animation::StartCard(int slot)
{
    Card& card = *m_dealStack->RemoveTopCard();

    auto moveStack = std::make_unique<CardStack>(CardStack::Type::Drag);

    moveStack->setPosition(card.getPosition());
    moveStack->AddCard(card);

    std::cout << "Move Stack" << std::endl;
    m_spiderAnimator.SimpleStackMove(std::move(moveStack), m_game.getStack(slot),
                    [this, slot]() { this->DoneMoveCard(slot); });
}


void Deal10Animation::DoneMoveCard(int slot)
{
    Card& card = m_game.getStack(slot).TopCard();
    m_spiderAnimator.TurnCard(card, true, [this, slot]() { this->DoneTurnSrcCard(slot); });
}


void Deal10Animation::DoneTurnSrcCard(int slot)
{
    CardStack& stack = m_game.getStack(slot);
    stack.ReevaluateMovability();

    // pause a bit before removing the full stacks.
    int delay = 500;

    if (stack.HasFullPack())
    {
        m_spiderAnimator.PackRemove(m_game, stack, [this, slot]() {this->DonePackRemoved(slot); },
                            ms_delayBeforeRemovingFullStack);
        m_record.packsRemoved[slot] = true;
        return;
    }
    DonePackRemoved(-1);
}


void Deal10Animation::DonePackRemoved(int slot)
{
    if (slot >= 0)
    {
        CardStack& stack = m_game.getStack(slot);
        if (!stack.IsEmpty() && !stack.TopCard().IsFaceUp())
        {
            m_spiderAnimator.TurnCard(stack.TopCard(), true, [this, slot]() { this->DoneTurnDestCard(slot); });
            m_record.cardsTurned[slot] = true;
            return;
        }
    }
    DoneTurnDestCard(-1);
}


void Deal10Animation::DoneTurnDestCard(int slot)
{
    if (--m_doneCount > 0)
        return;

    if (m_dealMoveCompletionCB != nullptr)
        m_dealMoveCompletionCB(m_record);

    BaseDone();
}
