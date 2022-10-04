#pragma once

#include <glm/glm.hpp>

#include <memory>


class Sprite;
class SpriteRenderer;
class Card;
class CardStack;
class Button;
class IGameMove;


namespace MyGL {
    class Window2D;
    class MouseMovedEvent;
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
}


class MouseInput
{
public:
    MouseInput(MyGL::Window2D& win, SpriteRenderer& spriteRenderer, IGameMove& gamemove);

    bool OnMouseMoved(MyGL::MouseMovedEvent& evt);
    bool OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt);
    bool OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt);

    bool OnCardPressed(Card& card);
    bool OnDragStackReleased();
    bool OnButtonPressed(Button& button);
    bool OnButtonReleased(Button& button);


private:
    MyGL::Window2D& m_win;
    SpriteRenderer& m_spriteRenderer;
    IGameMove& m_gameMove;

    glm::vec2 m_mousePos;

    std::unique_ptr<CardStack> m_dragStack;
    CardStack* m_dragSourceStack;
    glm::vec2 m_dragStartPos;
    glm::vec2 m_dragCardOffset;

    Button* m_buttonPushed;
};