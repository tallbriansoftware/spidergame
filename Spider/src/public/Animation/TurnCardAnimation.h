#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"

#include <memory>

class Card;

class TurnCardAnimation : public BaseAnimation
{
public:
    using DoneCB = std::function<void(TurnCardAnimation& turnAnimation)>;

public:
    TurnCardAnimation(SpiderAnimator& spiderAnimator, Card& card, bool turnUp);
    ~TurnCardAnimation();

    void setDuration(int durationMilliSec);

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void TurnAngle(float value);
    void DoneHalfTurn();
    void DoneFullTurn();


private:
    Card& m_card;
    bool m_turnUp;
    int m_duration;
};
