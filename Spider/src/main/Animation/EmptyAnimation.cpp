#include "Animation/EmptyAnimation.h"

#include "Animation/SpiderAnimator.h"

// Empty Animation:  Used to insert delays.

EmptyAnimation::EmptyAnimation(SpiderAnimator& spiderAnimator)
    : BaseAnimation(spiderAnimator) 
{
}

EmptyAnimation::~EmptyAnimation()
{
}

void EmptyAnimation::SetCompletionCallback(Animation::CompletionCB doneCB)
{
    m_completionCB = doneCB;
}

void EmptyAnimation::Start(int delayMilliSeconds)
{
    Animator& animator = m_spiderAnimator.getAnimator();

    auto turnFirstHalf = animator.CreateAnimation(0.0f, 1.0f, 1,
        [this](float value) {},
        [this]() {this->BaseDone(); });

    animator.StartAnimation(*turnFirstHalf, delayMilliSeconds);
}

