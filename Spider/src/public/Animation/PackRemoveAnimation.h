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


class PackRemoveAnimation : public BaseAnimation
{
public:
    PackRemoveAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, CardStack& packStack);
    ~PackRemoveAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void OneCardDone();

private:
    SpiderGame& m_game;
    CardStack& m_packStack;
    int m_doneCount;
    std::array<std::unique_ptr<CardStack>, 13> m_holdingStacks;
};
