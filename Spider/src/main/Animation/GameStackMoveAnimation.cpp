#include "Animation/GameStackMoveAnimation.h"

#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "App/SpiderGame.h"
#include "View/Card.h"
#include "View/CardStack.h"


GameStackMoveAnimation::GameStackMoveAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, const StackMoveRecord& rec, std::unique_ptr<CardStack> dropStack)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_record(rec)
    , m_dropStack(std::move(dropStack))
    , m_stackMoveCompletionCB(nullptr)
{
}

GameStackMoveAnimation::~GameStackMoveAnimation()
{
}


void GameStackMoveAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    assert("cannot set this callback on this animation" == nullptr);
}

void GameStackMoveAnimation::SetCompletionCallback(GameStackMoveAnimation::CompletionCB doneCB)
{
    m_stackMoveCompletionCB = doneCB;
}

void GameStackMoveAnimation::Start(int delayMilliSeconds)
{
    CardStack& targetStack = m_game.getStack(m_record.destStack);
    m_spiderAnimator.SimpleStackMove(std::move(m_dropStack), targetStack, [this]() {this->Step2_CheckTurnSourceCard(); }, delayMilliSeconds);
}

void GameStackMoveAnimation::Step2_CheckTurnSourceCard()
{
    CardStack& srcStack = m_game.getStack(m_record.srcStack);
    if (srcStack.getCountOfCards() > 0 && !srcStack.TopCard().IsFaceUp())
    {
        m_record.srcTurned = true;
        m_spiderAnimator.TurnCard(srcStack.TopCard(), true, [this]() { this->Step3_CheckRemoveFullPack(); });
        return;
    }
    Step3_CheckRemoveFullPack();
}

void GameStackMoveAnimation::Step3_CheckRemoveFullPack()
{
    CardStack& destStack = m_game.getStack(m_record.destStack);
    if (destStack.HasFullPack())
    {
        // delay a bit before removing the pack.
        int delay = 300;
        m_record.destRemoved = true;
        m_spiderAnimator.PackRemove(m_game, m_game.getStack(m_record.destStack), [this]() {this->Step4_CheckTurnDestCard(); }, delay);
        return;
    }

    Step4_CheckTurnDestCard();
}

void GameStackMoveAnimation::Step4_CheckTurnDestCard()
{
    CardStack& destStack = m_game.getStack(m_record.destStack);
    if (!destStack.IsEmpty() && !destStack.TopCard().IsFaceUp())
    {
        m_record.destTurned = true;
        m_spiderAnimator.TurnCard(destStack.TopCard(), true, [this]() {this->Done(); });
        return;
    }
    Done();
}

void GameStackMoveAnimation::Done()
{
    if (m_stackMoveCompletionCB != nullptr)
        m_stackMoveCompletionCB(m_record);

    BaseDone();
}