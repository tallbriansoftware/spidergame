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
    Deal10_UndoAnimation(SpiderAnimator& spiderAnimator, SpiderGame& game, const DealMoveRecord& record);
    ~Deal10_UndoAnimation();

    void SetCompletionCallback(Animation::CompletionCB doneCB) override;
    void Start(int delayMilliSeconds = 0) override;
    void Start_old(int delayMilliSeconds = 0);

private:
    void Step1_TurnCardsDown();
    void Done_TurningCardDown();
    void Step2_RestoreRemovedPacks();
    void Done_RestoreRemovedPack();
    void Step3_RemoveDeal();

    void MoveCard(int slot);
    void OneCardDone();

private:
    SpiderGame& m_game;
    DealMoveRecord m_record;
    int m_delayMilliSeconds;
    int m_doneTurnDownCount;
    int m_doneRestoredPackCount;
    int m_doneCount;
    std::array<std::unique_ptr<CardStack>, 10> m_holdingStacks;
};
