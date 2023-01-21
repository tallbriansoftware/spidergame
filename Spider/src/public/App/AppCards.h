#pragma once

#include "Application.h"

#include <array>
#include <memory>
#include <vector>
#include <functional>

class Animator;
class Button;
class Card;
class CardStack;
class GameResources;
class GameTime;
class MouseInput;
class SpiderAnimator;
class SpiderGame;
class SpriteFactory;
class SpriteRenderer;

namespace MyGL {
	class MouseMovedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
}


class AppCards : public Application
{
public:
	AppCards(const std::string& name = "OpenGL-Cards", uint32_t width = 1600, uint32_t height = 1000);
	~AppCards();

public:
	void Init() override;
	void Render() override;

protected:
	bool OnMousedMoved(MyGL::MouseMovedEvent& evt) override;
	bool OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt) override;
	bool OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt) override;

private:
	void OnUndoButtonClicked();
	void OnTest1ButtonClicked();
	void OnNewGameButtonClicked();
	void OnHintButtonClicked();

	void InitButton(int position, const std::string& name, const std::function<void()>& callback);

private:
	std::unique_ptr<GameResources> m_gameResources;
	std::unique_ptr<SpriteFactory> m_spriteFactory;
	std::unique_ptr<SpriteRenderer> m_spriteRenderer;
	std::unique_ptr<MouseInput> m_mouseInput;
	std::unique_ptr<SpiderGame> m_spiderGame;
	std::unique_ptr<GameTime> m_gameTime;
	std::unique_ptr<Animator> m_animator;
	std::unique_ptr<SpiderAnimator> m_spiderAnimator;

	std::vector<std::shared_ptr<Card>> m_cards;
	std::array<std::shared_ptr<CardStack>, 10> m_stacks;
	std::shared_ptr<CardStack> m_stock;
	std::shared_ptr<CardStack> m_donePacks;
};
