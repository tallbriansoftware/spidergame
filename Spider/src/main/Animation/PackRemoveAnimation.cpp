#include "Animation/PackRemoveAnimation.h"

#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "App/SpiderGame.h"
#include "View/Card.h"
#include "View/CardStack.h"

#include <assert.h>

PackRemoveAnimation::PackRemoveAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, CardStack& packStack)
    : BaseAnimation(spiderAnimator)
    , m_game(game)
    , m_packStack(packStack)
    , m_doneCount(-1)
{
    int count = m_game.getDonePacks().getCountOfCards();
    for (int i = 0; i < 13; i++)
    {
        m_holdingStacks[i] = std::make_unique<CardStack>(CardStack::Type::Mock);
        m_holdingStacks[i]->setPosition(m_game.getDonePacks().getPositionOfCard(count + i, true));
    }
}


PackRemoveAnimation::~PackRemoveAnimation()
{
}


void PackRemoveAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}

void PackRemoveAnimation::Start(int delayMilliSeconds)
{
    m_doneCount = 13;
    std::vector<Card*> pack = m_packStack.RemoveFullPack();

    for(int i=0; i<13; i++)
    {
        Card& card = *pack[i];

        int delay = delayMilliSeconds +  i * 5;

        auto moveStack = std::make_unique<CardStack>(CardStack::Type::Drag);
        moveStack->setPosition(card.getPosition());
        moveStack->AddCard(card);

        m_spiderAnimator.SimpleStackMove(std::move(moveStack), *m_holdingStacks[i], [this]() { this->OneCardDone(); }, delay);
    }
}

void PackRemoveAnimation::OneCardDone()
{
    if (--m_doneCount > 0)
        return;

    for (int i = 0; i < 13; ++i)
    {
        m_game.getDonePacks().AddCard(*m_holdingStacks[i]->RemoveTopCard());
        assert(m_holdingStacks[i]->getCountOfCards() == 0);
    }

    BaseDone();
}
