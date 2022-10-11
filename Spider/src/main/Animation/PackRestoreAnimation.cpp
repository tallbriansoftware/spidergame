#include "Animation/PackRestoreAnimation.h"

#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "App/SpiderGame.h"
#include "View/Card.h"
#include "View/CardStack.h"


PackRestoreAnimation::PackRestoreAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, int stackNo)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_stackNo(stackNo)
    , m_doneCount(-1)
{
    CardStack& stack = m_game.getStack(m_stackNo);
    int count = stack.getCountOfCards();
    for (int i = 0; i < 13; i++)
    {
        m_holdingStacks[i] = std::make_unique<CardStack>(CardStack::Type::Mock);
        m_holdingStacks[i]->setPosition(stack.getPositionOfCard(count + i, true));
    }
}


PackRestoreAnimation::~PackRestoreAnimation()
{
}


void PackRestoreAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}


void PackRestoreAnimation::Start(int delayMilliSeconds)
{
    CardStack& packStack = m_game.getDonePacks();
    CardStack& destStack = m_game.getStack(m_stackNo);

    std::vector<Card*> cards = packStack.RemoveCards(packStack.getCountOfCards() - 13);
    m_doneCount = 13;
    for (int i=0; i<13; ++i)
    {
        Card& card = *cards[i];

        int delay = delayMilliSeconds + i * 5;

        auto movingStack = std::make_unique<CardStack>(CardStack::Type::Drag);
        movingStack->setPosition(card.getPosition());
        movingStack->AddCard(card);

        m_spiderAnimator.SimpleStackMove(std::move(movingStack), *m_holdingStacks[i], [this]() { this->OneCardDone(); }, delay);
    }
}


void PackRestoreAnimation::OneCardDone()
{
    if (--m_doneCount > 0)
        return;

    CardStack& stack = m_game.getStack(m_stackNo);

    for (int i = 0; i < 13; ++i)
    {
        Card& card = *m_holdingStacks[i]->RemoveTopCard();
        card.BringToTop();
        stack.AddCard(card);
        assert(m_holdingStacks[i]->getCountOfCards() == 0);
    }

    BaseDone();
}

