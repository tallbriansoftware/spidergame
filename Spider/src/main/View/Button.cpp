#include "View/Button.h"

#include "View/SpriteFactory.h"


Button::Button(SpriteFactory* factory)
    : Sprite(SpriteType::Button, *factory)
    , m_onClick(nullptr)
{
}


void Button::LoadImage(const std::string& name)
{
    m_name = name;
    m_factory.SetGraphicsFromButtonName(name, *this);
}


std::string Button::getName()
{
    return std::string();
}

void Button::setOnClick(const ButtonClickCalllback& callback)
{
    m_onClick = callback;
}

void Button::OnClick()
{
    m_onClick();
}
