#include "Animation/SpiderAnimator.h"

#include "Animation/GameStackMoveAnimation.h"
#include "Animation/GameStackMove_UndoAnimation.h"
#include "Animation/SpiderAnimator.h"
#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/TurnCardAnimation.h"
#include "Animation/PackRemoveAnimation.h"
#include "Animation/PackRestoreAnimation.h"
#include "Animation/Deal10Animation.h"
#include "Animation/Deal10_UndoAnimation.h"
#include "Animation/DealGameAnimation.h"
#include "Animation/EmptyAnimation.h"
#include "App/SpiderGame.h"
#include "Model/MoveRecord.h"
#include "View/Card.h"
#include "View/CardStack.h"

#include "glm/glm.hpp"

#include <memory>
#include <iostream>


namespace
{
    const int ms_defaultTurnSpeed = 300;
    const float pxms_defaultMoveSpeed = 5.0f;
}

SpiderAnimator::SpiderAnimator(Animator& animator)
    : m_animator(animator)
    , m_cardTurnMilliSeconds(ms_defaultTurnSpeed)
    , m_cardMoveSpeed(pxms_defaultMoveSpeed)
{
}


SpiderAnimator::~SpiderAnimator()
{
}

Animator& SpiderAnimator::getAnimator() const
{
    return m_animator;
}

void SpiderAnimator::Empty(Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    EmptyAnimation* animation = CreateEmptyAnimation();
    animation->SetCompletionCallback(completionCB);
    animation->Start(delayMilliSeconds);
}

void SpiderAnimator::TurnCard(Card& card, bool turnUp,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    TurnCardAnimation* animation = CreateTurnCardAnimation(card, turnUp);
    animation->setDuration(m_cardTurnMilliSeconds);
    animation->SetCompletionCallback(completionCB);
    animation->Start(delayMilliSeconds);
}


void SpiderAnimator::SimpleStackMove(std::unique_ptr<CardStack> dragStack, CardStack& target,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    SimpleStackMoveAnimation* stackAnimation = CreateSimpleStackMoveAnimation(std::move(dragStack), target);
    stackAnimation->setSpeed(m_cardMoveSpeed);
    stackAnimation->SetCompletionCallback(completionCB);
    stackAnimation->Start(delayMilliSeconds);
}

void SpiderAnimator::GameStackMove(SpiderGame& game, const StackMoveRecord& record, std::unique_ptr<CardStack> dropStack,
    GameStackMoveAnimation::CompletionCB completionCB, int delayMilliSeconds)
{
    GameStackMoveAnimation* stackAnimation = CreateGameStackMoveAnimation(game, record, std::move(dropStack));
    stackAnimation->SetCompletionCallback(completionCB);
    stackAnimation->Start(delayMilliSeconds);
}


void SpiderAnimator::GameStackMove_Undo(SpiderGame& game, const StackMoveRecord& record,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    GameStackMove_UndoAnimation* stackAnimation = CreateGameStackMove_UndoAnimation(game, record);
    stackAnimation->SetCompletionCallback(completionCB);
    stackAnimation->Start(delayMilliSeconds);
}


void SpiderAnimator::PackRemove(SpiderGame& game, CardStack& packStack,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    PackRemoveAnimation* packRemove = CreatePackRemoveAnimation(game, packStack);
    packRemove->SetCompletionCallback(completionCB);
    packRemove->Start(delayMilliSeconds);
}

void SpiderAnimator::PackRestore(SpiderGame& game, int destStack,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    PackRestoreAnimation* packRestore = CreatePackRestoreAnimation(game, destStack);
    packRestore->SetCompletionCallback(completionCB);
    packRestore->Start(delayMilliSeconds);
}


void SpiderAnimator::Deal10(SpiderGame& game,
    Deal10Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    Deal10Animation* dealAnimation = CreateDeal10Animation(game);
    dealAnimation->SetCompletionCallback(completionCB);
    dealAnimation->Start(delayMilliSeconds);

}


void SpiderAnimator::Deal10_Undo(SpiderGame& game,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    Deal10_UndoAnimation* dealUndoAnimation = CreateDeal10_UndoAnimation(game);
    dealUndoAnimation->SetCompletionCallback(completionCB);
    dealUndoAnimation->Start(delayMilliSeconds);

}


void SpiderAnimator::DealGame(SpiderGame& game,
    Animation::CompletionCB completionCB, int delayMilliSeconds)
{
    DealGameAnimation* dealAnimation = CreateDealGameAnimation(game);
    dealAnimation->SetCompletionCallback(completionCB);
    dealAnimation->Start(delayMilliSeconds);
}

// ====================== Create Animations ==============================

EmptyAnimation* SpiderAnimator::CreateEmptyAnimation()
{
    auto animation_ptr = std::make_unique<EmptyAnimation>(*this);
    m_animations.push_back(std::move(animation_ptr));
    EmptyAnimation* animation = dynamic_cast<EmptyAnimation*>(m_animations.back().get());
    return animation;
}


TurnCardAnimation* SpiderAnimator::CreateTurnCardAnimation(Card& card, bool turnUp)
{
    auto turnAnimation_ptr = std::make_unique<TurnCardAnimation>(*this, card, turnUp);
    m_animations.push_back(std::move(turnAnimation_ptr));
    TurnCardAnimation* turnAnimation = dynamic_cast<TurnCardAnimation*>(m_animations.back().get());
    return turnAnimation;
}


SimpleStackMoveAnimation* SpiderAnimator::CreateSimpleStackMoveAnimation(std::unique_ptr<CardStack> dropStack, CardStack& target)
{
    auto stackAnimation_ptr = std::make_unique<SimpleStackMoveAnimation>(*this, std::move(dropStack), target);
    m_animations.push_back(std::move(stackAnimation_ptr));
    SimpleStackMoveAnimation* stackAnimation = dynamic_cast<SimpleStackMoveAnimation*>(m_animations.back().get());
    return stackAnimation;
}


GameStackMoveAnimation* SpiderAnimator::CreateGameStackMoveAnimation(SpiderGame& game, const StackMoveRecord& rec, std::unique_ptr<CardStack> dropStack)
{
    auto gameStackAnimation_ptr = std::make_unique<GameStackMoveAnimation>(*this, game, rec, std::move(dropStack));
    m_animations.push_back(std::move(gameStackAnimation_ptr));
    GameStackMoveAnimation* gameStackAnimation = dynamic_cast<GameStackMoveAnimation*>(m_animations.back().get());
    return gameStackAnimation;
}


GameStackMove_UndoAnimation* SpiderAnimator::CreateGameStackMove_UndoAnimation(SpiderGame& game, const StackMoveRecord& rec)
{
    auto gameStack_UndoAnimation_ptr = std::make_unique<GameStackMove_UndoAnimation>(*this, game, rec);
    m_animations.push_back(std::move(gameStack_UndoAnimation_ptr));
    GameStackMove_UndoAnimation* gameStack_UndoAnimation = dynamic_cast<GameStackMove_UndoAnimation*>(m_animations.back().get());
    return gameStack_UndoAnimation;
}


PackRemoveAnimation* SpiderAnimator::CreatePackRemoveAnimation(SpiderGame& game, CardStack& packStack)
{
    auto packRemoveAnimation_ptr = std::make_unique<PackRemoveAnimation>(*this, game, packStack);
    m_animations.push_back(std::move(packRemoveAnimation_ptr));
    PackRemoveAnimation* packRemoveAnimation = dynamic_cast<PackRemoveAnimation*>(m_animations.back().get());
    return packRemoveAnimation;
}


PackRestoreAnimation* SpiderAnimator::CreatePackRestoreAnimation(SpiderGame& game, int destStack)
{
    auto packRemoveAnimation_ptr = std::make_unique<PackRestoreAnimation>(*this, game, destStack);
    m_animations.push_back(std::move(packRemoveAnimation_ptr));
    PackRestoreAnimation* packRestoreAnimation = dynamic_cast<PackRestoreAnimation*>(m_animations.back().get());
    return packRestoreAnimation;
}

Deal10Animation* SpiderAnimator::CreateDeal10Animation(SpiderGame& game)
{
    auto deal10Animation_ptr = std::make_unique<Deal10Animation>(*this, game);
    m_animations.push_back(std::move(deal10Animation_ptr));
    Deal10Animation* dealAnimation = dynamic_cast<Deal10Animation*>(m_animations.back().get());
    return dealAnimation;
}

Deal10_UndoAnimation* SpiderAnimator::CreateDeal10_UndoAnimation(SpiderGame& game)
{
    auto deal10_UndoAnimation_ptr = std::make_unique<Deal10_UndoAnimation>(*this, game);
    m_animations.push_back(std::move(deal10_UndoAnimation_ptr));
    Deal10_UndoAnimation* deal_UndoAnimation = dynamic_cast<Deal10_UndoAnimation*>(m_animations.back().get());
    return deal_UndoAnimation;
}

DealGameAnimation* SpiderAnimator::CreateDealGameAnimation(SpiderGame& game)
{
    auto dealGameAnimation_ptr = std::make_unique<DealGameAnimation>(*this, game);
    m_animations.push_back(std::move(dealGameAnimation_ptr));
    DealGameAnimation* dealAnimation = dynamic_cast<DealGameAnimation*>(m_animations.back().get());
    return dealAnimation;
}


void SpiderAnimator::CompleteAnimation(BaseAnimation& stackAnimation)
{
    auto itr = std::find_if(m_animations.begin(), m_animations.end(),
        [&stackAnimation](std::unique_ptr<BaseAnimation>& anim) {
            return &stackAnimation == anim.get(); });

    assert(itr != m_animations.end());
    m_animations.erase(itr);
}
