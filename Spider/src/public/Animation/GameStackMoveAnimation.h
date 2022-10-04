#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"
#include "Model/MoveRecord.h"

#include <functional>
#include <memory>

class CardStack;
class SpiderAnimator;
class SpiderGame;


class GameStackMoveAnimation : public BaseAnimation
{
public:
    // CompletionCB: Called when the animation is done.  (delivers a Move Record)
    using CompletionCB = std::function<void(const StackMoveRecord& record)>;

public:
    GameStackMoveAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, const StackMoveRecord& rec, std::unique_ptr<CardStack> dropStack);
    ~GameStackMoveAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void SetCompletionCallback(GameStackMoveAnimation::CompletionCB doneCB);
    void Start(int delayMilliSeconds = 0) override;

private:
    void Step2_CheckTurnSourceCard();
    void Step3_CheckRemoveFullPack();
    void Step4_CheckTurnDestCard();
    void Done();

private:
    SpiderGame& m_game;
    StackMoveRecord m_record;
    std::unique_ptr<CardStack> m_dropStack;
    CompletionCB m_stackMoveCompletionCB;
};