#include "Animation/SimpleStackMoveAnimation.h"
#include "Animation/SpiderAnimator.h"


#include "Animate/Animation.h"
#include "Animate/Animator.h"

#include "View/Card.h"
#include "View/CardStack.h"

#include <iostream>
#include <cmath>


SimpleStackMoveAnimation::SimpleStackMoveAnimation(SpiderAnimator& spiderAnimator, std::unique_ptr<CardStack> dragStack, CardStack& target)
    : BaseAnimation(spiderAnimator)
    , m_dragStack(std::move(dragStack))
    , m_targetStack(target)
    , m_doneCount(0)
    , m_speed(1.0f)
{
}


SimpleStackMoveAnimation::~SimpleStackMoveAnimation()
{
}


void SimpleStackMoveAnimation::setSpeed(float pxPerMilliSecond)
{
    m_speed = pxPerMilliSecond;
}


void SimpleStackMoveAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}

void SimpleStackMoveAnimation::Start(int delayMilliSeconds)
{
    Animation::MotionCB motionX = [this](float value) { this->MoveByX(value); };
    Animation::MotionCB motionY = [this](float value) { this->MoveByY(value); };

    Animation::CompletionCB complete = [this]() { this->PartDone(this->m_targetStack); };

    glm::vec2 startCoords = this->m_dragStack->getPosition();
    glm::vec2 endCoords = m_targetStack.getPositionOfNextCard();

    float dx = startCoords.x - endCoords.x;
    float dy = startCoords.y - endCoords.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    int duration_msec = (int)(distance / m_speed);

    Animator& animator = m_spiderAnimator.getAnimator();

    auto moveX = animator.CreateAnimation(startCoords.x, endCoords.x, duration_msec, motionX, complete);
    auto moveY = animator.CreateAnimation(startCoords.y, endCoords.y, duration_msec, motionY, complete);

    m_doneCount = 2;
    animator.StartAnimation(*moveX, delayMilliSeconds);
    animator.StartAnimation(*moveY, delayMilliSeconds);
}


void SimpleStackMoveAnimation::MoveByX(float value)
{
    auto pos = m_dragStack->getPosition();
    pos.x = value;
    m_dragStack->setPosition(pos);
}


void SimpleStackMoveAnimation::MoveByY(float value)
{
    auto pos = m_dragStack->getPosition();
    pos.y = value;
    m_dragStack->setPosition(pos);
}


void SimpleStackMoveAnimation::PartDone(CardStack& target)
{
    if (--m_doneCount > 0)
        return;

    target.DoDrop(*this->m_dragStack);
    BaseDone();
}

