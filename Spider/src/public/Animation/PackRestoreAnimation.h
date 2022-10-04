#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"
#include "Model/MoveRecord.h"

#include <functional>
#include <memory>
#include <array>

class CardStack;
class SpiderAnimator;
class SpiderGame;


class PackRestoreAnimation : public BaseAnimation
{
public:
    PackRestoreAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, int stackNo);
    ~PackRestoreAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void OneCardDone();

private:
    SpiderGame& m_game;
    int m_stackNo;
    int m_doneCount;
    std::array<std::unique_ptr<CardStack>, 13> m_holdingStacks;
};
