#include "Animation/GameStackMove_UndoAnimation.h"

#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "App/SpiderGame.h"
#include "View/Card.h"
#include "View/CardStack.h"


GameStackMove_UndoAnimation::GameStackMove_UndoAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, const StackMoveRecord& rec)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_record(rec)
{
}


GameStackMove_UndoAnimation::~GameStackMove_UndoAnimation()
{
}


void GameStackMove_UndoAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}


void GameStackMove_UndoAnimation::Start(int delayMilliSeconds)
{
    // This is an undo opeation.
    // The src and dest retain their original names
    // but the "sense" is reversed here.

    if (m_record.destTurned)
    {
        m_spiderAnimator.TurnCard(m_game.getStack(m_record.destStack).TopCard(), false, [this]() {this->Step2_RestoreRemovedPacks(); });
        return;
    }
    Step2_RestoreRemovedPacks();
}


void GameStackMove_UndoAnimation::Step2_RestoreRemovedPacks()
{
    if (m_record.destRemoved)
    {
        m_spiderAnimator.PackRestore(m_game, m_record.destStack, [this]() {this->Step3_TurnDownSource(); });
        return;
    }
    Step3_TurnDownSource();
}


void GameStackMove_UndoAnimation::Step3_TurnDownSource()
{
    if (m_record.srcTurned)
    {
        m_spiderAnimator.TurnCard(m_game.getStack(m_record.srcStack).TopCard(), false, [this]() {this->Step4_MoveBack(); });
        return;
    }
    Step4_MoveBack();
}


void GameStackMove_UndoAnimation::Step4_MoveBack()
{
    // This is an undo opeation.
    // The src and dest retain their original names
    // but the "sense" is reversed here.

    CardStack& destStack = m_game.getStack(m_record.destStack);
    CardStack& srcStack = m_game.getStack(m_record.srcStack);

    std::vector<Card*> cards = destStack.RemoveCards(m_record.destCard);

    auto dragStack = std::make_unique<CardStack>(CardStack::Type::Drag);
    dragStack->setPosition(cards[0]->getPosition());

    for (auto card : cards)
    {
        dragStack->AddCard(*card);
        card->BringToTop();
    }

    m_spiderAnimator.SimpleStackMove(std::move(dragStack), srcStack, [this]() { this->Done(); });

}

void GameStackMove_UndoAnimation::Done()
{
    BaseDone();
}

