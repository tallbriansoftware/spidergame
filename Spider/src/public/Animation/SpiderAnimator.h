#pragma once

#include "Animate/Animation.h"
#include "Animation/GameStackMoveAnimation.h"
#include "Animation/Deal10Animation.h"

#include <memory>
#include <list>
#include <vector>

class Animator;
class Card;
class CardStack;
class SpiderGame;

class StackMoveRecord;
class DealMoveRecord;

class BaseAnimation;
class EmptyAnimation;
class SimpleStackMoveAnimation;
class TurnCardAnimation;
class PackRemoveAnimation;
class PackRestoreAnimation;
class DealGameAnimation;
class GameStackMove_UndoAnimation;
class Deal10_UndoAnimation;


class SpiderAnimator
{
public:
    SpiderAnimator(Animator& animator);
    ~SpiderAnimator();

    Animator& getAnimator() const;

    void Empty(Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void TurnCard(Card& card, bool turnUp,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void SimpleStackMove(std::unique_ptr<CardStack> dragStack, CardStack& dest,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void GameStackMove(SpiderGame& game, const StackMoveRecord& record, std::unique_ptr<CardStack> dropStck,
        GameStackMoveAnimation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void GameStackMove_Undo(SpiderGame& game, const StackMoveRecord& record,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void PackRemove(SpiderGame& game, CardStack& packStack,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void PackRestore(SpiderGame& game, int srcStack,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void Deal10(SpiderGame& game,
        Deal10Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void Deal10_Undo(SpiderGame& game, const DealMoveRecord& record,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);
    void DealGame(SpiderGame& game,
        Animation::CompletionCB completionCB = nullptr, int delayMilliSeconds = 0);


    void CompleteAnimation(BaseAnimation& turnAnimation);

private:
    EmptyAnimation* CreateEmptyAnimation();
    TurnCardAnimation* CreateTurnCardAnimation(Card& card, bool turnUp);
    SimpleStackMoveAnimation* CreateSimpleStackMoveAnimation(std::unique_ptr<CardStack> dropStack, CardStack& target);
    GameStackMoveAnimation* CreateGameStackMoveAnimation(SpiderGame& game, const StackMoveRecord& rec, std::unique_ptr<CardStack> dropStack);
    GameStackMove_UndoAnimation* CreateGameStackMove_UndoAnimation(SpiderGame& game, const StackMoveRecord& rec);
    PackRemoveAnimation* CreatePackRemoveAnimation(SpiderGame& game, CardStack& packStack);
    PackRestoreAnimation* CreatePackRestoreAnimation(SpiderGame& game, int destStack);
    Deal10Animation* CreateDeal10Animation(SpiderGame& game);
    Deal10_UndoAnimation* CreateDeal10_UndoAnimation(SpiderGame& game, const DealMoveRecord& record);
    DealGameAnimation* CreateDealGameAnimation(SpiderGame& game);

private:
    int m_cardTurnMilliSeconds;
    float m_cardMoveSpeed;  //  px / millisecond;
    Animator& m_animator;
    std::vector<std::unique_ptr<BaseAnimation>> m_animations;
};
