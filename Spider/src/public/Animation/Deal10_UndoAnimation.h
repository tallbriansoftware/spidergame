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


class Deal10_UndoAnimation : public BaseAnimation
{
public:
    Deal10_UndoAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game);
    ~Deal10_UndoAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void MoveCard(int slot);
    void OneCardDone();

private:
    SpiderGame& m_game;
    int m_doneCount;
    std::array<std::unique_ptr<CardStack>, 10> m_holdingStacks;
};
