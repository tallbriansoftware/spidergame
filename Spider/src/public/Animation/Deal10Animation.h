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


class Deal10Animation : public BaseAnimation
{
public:
    // CompletionCB: Called when the animation is done.  (delivers a Move Record)
    using CompletionCB = std::function<void(const DealMoveRecord& record)>;

public:
    Deal10Animation(SpiderAnimator& spiderAnimator, SpiderGame& game);
    ~Deal10Animation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void SetCompletionCallback(Deal10Animation::CompletionCB doneCB);
    void Start(int delayMilliSeconds = 0) override;

private:
    void StartCard(int slot);
    void DoneMoveCard(int slot);
    void DoneTurnSrcCard(int slot);
    void DonePackRemoved(int slot);
    void DoneTurnDestCard(int slot);

private:
    SpiderGame& m_game;
    int m_doneCount;
    DealMoveRecord m_record;
    CompletionCB m_dealMoveCompletionCB;
    std::unique_ptr<CardStack> m_dealStack;
};