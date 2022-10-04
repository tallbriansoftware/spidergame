#include "Animation/BaseAnimation.h"

#include "Animation/SpiderAnimator.h"


BaseAnimation::BaseAnimation(SpiderAnimator& spiderAnimator)
    : m_spiderAnimator(spiderAnimator)
    , m_completionCB(nullptr)
{
}


BaseAnimation::~BaseAnimation()
{
}


void BaseAnimation::BaseDone()
{
    if (m_completionCB != nullptr)
        m_completionCB();

    // Notify the Animator that this Animation is finished.
    m_spiderAnimator.CompleteAnimation(*this);
}