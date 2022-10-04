#pragma once

#include <Animate/Animator.h>

#include <vector>

class SpiderAnimator;

class BaseAnimation
{
public:
    BaseAnimation(SpiderAnimator& spiderAnimator);
    virtual ~BaseAnimation();

    virtual void SetCompletionCallback(Animation::CompletionCB doneCB) = 0;
    virtual void Start(int delayMilliSeconds = 0) = 0;
    void BaseDone();

protected:
    SpiderAnimator& m_spiderAnimator;
    Animation::CompletionCB m_completionCB;

private:
};