#include "Animation/Deal10_UndoAnimation.h"

#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "App/SpiderGame.h"
#include "View/Card.h"
#include "View/CardStack.h"


Deal10_UndoAnimation::Deal10_UndoAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_doneCount(-1)
{
    int count = m_game.getStock().getCountOfCards();
    for (int i = 0; i < 10; i++)
    {
        m_holdingStacks[i] = std::make_unique<CardStack>(CardStack::Type::Mock);
        m_holdingStacks[i]->setPosition(m_game.getStock().getPositionOfCard(count + i, false));
    }
}


Deal10_UndoAnimation::~Deal10_UndoAnimation()
{
}


void Deal10_UndoAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}


void Deal10_UndoAnimation::Start(int delayMilliSeconds)
{
    m_doneCount = 10;
    std::vector<Card*> cardList;
    for (int slot = 0; slot < 10; ++slot)
    {
        int delay = delayMilliSeconds + slot * 20;
        Card& card = m_game.getStack(slot).TopCard();
        cardList.push_back(&card);
        m_spiderAnimator.TurnCard(card, false, [this, slot]() { this->MoveCard(slot); }, delay);
    }
    for (int i = (int)cardList.size() - 1; i >= 0; --i)
        cardList[i]->BringToTop();
}

void Deal10_UndoAnimation::MoveCard(int slot)
{
    m_doneCount = 10;
    Card& card = *m_game.getStack(slot).RemoveTopCard();

    auto moveStack = std::make_unique<CardStack>(CardStack::Type::Drag);
    moveStack->setPosition(card.getPosition());
    moveStack->AddCard(card);

    m_spiderAnimator.SimpleStackMove(std::move(moveStack), *m_holdingStacks[slot], [this]() { this->OneCardDone(); });
}


void Deal10_UndoAnimation::OneCardDone()
{
    if (--m_doneCount > 0)
        return;

    // restore to Stock in reverse order.
    for (int i = 9; i >= 0 ; --i)
    {
        Card& card = *m_holdingStacks[i]->RemoveTopCard();
        m_game.getStock().AddCard(card);
        assert(m_holdingStacks[i]->getCountOfCards() == 0);
    }

    BaseDone();
}
