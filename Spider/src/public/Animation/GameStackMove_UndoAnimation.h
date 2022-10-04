#pragma once

#include "Animation/BaseAnimation.h"

#include "Animate/Animation.h"
#include "Model/MoveRecord.h"

#include <functional>
#include <memory>

class CardStack;
class SpiderAnimator;
class SpiderGame;


class GameStackMove_UndoAnimation : public BaseAnimation
{
public:
    GameStackMove_UndoAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, const StackMoveRecord& rec);
    ~GameStackMove_UndoAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;

private:
    void Step2_RestoreRemovedPacks();
    void Step3_TurnDownSource();
    void Step4_MoveBack();
    void Done();

private:
    SpiderGame& m_game;
    StackMoveRecord m_record;
};