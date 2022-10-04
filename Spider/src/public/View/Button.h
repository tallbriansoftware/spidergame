#pragma once

#include "View/Sprite.h"

#include <string>
#include <functional>

class Button : public Sprite
{
public:
    using ButtonClickCalllback = std::function<void()>;

public:
    Button(SpriteFactory* factory);
    void LoadImage(const std::string& name);
    std::string getName();
    void setOnClick(const ButtonClickCalllback& callback);
    void OnClick();

private:
    std::string m_name;
    ButtonClickCalllback m_onClick;
};
