#pragma once


#include <memory>
#include <vector>

#include <glm/glm.hpp>

class Sprite;
class StackSlot;
class CardStack;
class Card;
class Sprite;


namespace MyGL {
    class Window2D;
}

class SpriteRenderer
{
public:
    void AddButton(const std::shared_ptr<Sprite>& button);
    void RemoveButton(Sprite& button);

    void AddCard(const std::shared_ptr<Sprite>& card);
    void RemoveCard(Sprite& card);

    void AddStackSlot(const std::shared_ptr<StackSlot>& slot);
    void RemoveStackSlot(StackSlot& slot);

    void DrawSprites(const MyGL::Window2D& window2D) const;
    void BringToTop(Sprite& sprite);


    Sprite* HitTest(const MyGL::Window2D& window2D, const glm::vec2& testPos, const std::vector<Card*>* excludeList = nullptr) const;
    Sprite* HitTest(const MyGL::Window2D& window2D, const CardStack& dropStack) const;


private:
    // Hold shared pointers to keep everything alive
    std::vector<std::shared_ptr<Sprite>> m_spriteHolder;

    std::vector<Sprite*> m_buttonDrawList;
    std::vector<Sprite*> m_cardDrawList;
    std::vector<Sprite*> m_slotDrawList;
};