#pragma once

#include <memory>

class CardStack;
class Sprite;


class IGameMove
{
public:
    virtual void DropDragStack(std::unique_ptr<CardStack> dragStack, Sprite* target, CardStack& returnStack) = 0;
    virtual void Deal() = 0;
    virtual bool Undo() = 0;
};