#include "Animation/TurnCardAnimation.h"

#include "Animation/SpiderAnimator.h"

#include "Animate/Animator.h"

#include "View/Card.h"

#include <iostream>


TurnCardAnimation::TurnCardAnimation(SpiderAnimator& spiderAnimator, Card& card, bool turnUp)
    : BaseAnimation(spiderAnimator)
    , m_card(card)
    , m_turnUp(turnUp)
    , m_duration(200)
{
}

TurnCardAnimation::~TurnCardAnimation()
{
}


void TurnCardAnimation::setDuration(int durationMilliSec)
{
    m_duration = durationMilliSec;
}


void TurnCardAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}

void TurnCardAnimation::Start(int delayMilliSeconds)
{
    Animator& animator = m_spiderAnimator.getAnimator();

    auto turnFirstHalf = animator.CreateAnimation(0.0f, 90.0f, m_duration/2,
        [this](float value) { this->TurnAngle(value); },
        [this]() { this->DoneHalfTurn(); });

    animator.StartAnimation(*turnFirstHalf, delayMilliSeconds);
}


void TurnCardAnimation::TurnAngle(float value)
{
    m_card.setAngleDegrees(value);
}


void TurnCardAnimation::DoneHalfTurn()
{
    if (m_turnUp)
        m_card.TurnFaceUp();
    else
        m_card.TurnFaceDown();

    Animator& animator = m_spiderAnimator.getAnimator();

    auto turnAngle = animator.CreateAnimation(-90.0f, 0.0f, m_duration/2,
        [this](float value) { this->TurnAngle(value); },
        [this]() { this->DoneFullTurn(); });

    animator.StartAnimation(*turnAngle);
}


void TurnCardAnimation::DoneFullTurn()
{
    BaseDone();
}
