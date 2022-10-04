#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"

#include <functional>
#include <memory>
#include <array>

class CardStack;
class SpiderAnimator;
class SpiderGame;


class EmptyAnimation : public BaseAnimation
{
public:
    EmptyAnimation(SpiderAnimator& spiderAnimator);
    ~EmptyAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;
};