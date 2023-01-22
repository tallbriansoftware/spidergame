#pragma once

#include "App/IGameMove.h"

#include "Model/CardModel.h"

class Card;
class CardStack;
class SpiderAnimator;
class SpriteFactory;
class SpriteRenderer;
class MoveRecord;
class StackMoveRecord;
class DealMoveRecord;

#include <array>
#include <memory>
#include <vector>

class SpiderGame: public IGameMove
{
public:
    SpiderGame(SpriteFactory& SpriteFactory, SpriteRenderer& SpriteRender, SpiderAnimator& animation);
    ~SpiderGame();
    void Init(int numSuits, int seed=0);
    void Reshuffle();

    CardStack& getStack(int slot);
    CardStack& getStock();
    CardStack& getDonePacks();

    // IGameMove
    void DropDragStack(std::unique_ptr<CardStack> dragStack, Sprite* target, CardStack& returnStack) override;
    void Deal() override;
    bool Undo() override;

    bool Hint();
    void SetTestConfig(int num);

private:
    void Init_Cards();
    void Init_Stacks();
    void SetupStock(std::vector<Card*>& cards);

    std::vector<Card*> CollectAllCards();
    void DealGame(std::vector<Card*>& cards);

    bool Undo_StackMove(const StackMoveRecord& rec);
    bool Undo_Deal(const DealMoveRecord& rec);
    void Undo_Done();
    void StackMoveDone(const StackMoveRecord& rec);
    void DealMoveDone(const DealMoveRecord& rec);

    void SetStackTestConfig(std::vector<Card*>& allCards, const std::vector<CardModel>& source, CardStack& target);

    SpriteFactory& m_spriteFactory;
    SpriteRenderer& m_spriteRenderer;
    SpiderAnimator& m_spiderAnimator;
    bool m_busy;

    std::vector<std::shared_ptr<Card>> m_cards;
    std::array<std::unique_ptr<CardStack>, 10> m_stacks;
    std::unique_ptr<CardStack> m_stock;
    std::unique_ptr<CardStack> m_donePacks;
    std::vector<std::unique_ptr<MoveRecord>> m_undoStack;
};