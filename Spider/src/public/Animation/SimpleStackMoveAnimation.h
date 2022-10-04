#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"

#include "glm/glm.hpp"

#include <memory>

class CardStack;

class SimpleStackMoveAnimation : public BaseAnimation
{
public:
    SimpleStackMoveAnimation(SpiderAnimator& spiderAnimator, std::unique_ptr<CardStack> dragStack, CardStack& target);
    ~SimpleStackMoveAnimation();

    void setSpeed(float pxPerMilliSecond);

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void MoveByX(float value);
    void MoveByY(float value);
    void PartDone(CardStack& target);

private:
    std::unique_ptr<CardStack> m_dragStack;
    CardStack& m_targetStack;
    int m_doneCount;
    float m_speed;
};
