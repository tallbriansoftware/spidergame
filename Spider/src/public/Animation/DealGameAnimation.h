#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"

#include <functional>
#include <memory>
#include <array>

class CardStack;
class SpiderAnimator;
class SpiderGame;


class DealGameAnimation : public BaseAnimation
{
public:
    DealGameAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game);
    ~DealGameAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void DealOneCard(int slot, Animation::CompletionCB completionCB);
    void StartOneDownCard(int slot);
    void DoneMoveDownCard(int slot);
    void DoneTurnCard();

private:
    SpiderGame& m_game;
    int m_doneCount;
};