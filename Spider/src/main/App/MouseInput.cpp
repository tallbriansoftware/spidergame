#include "App/MouseInput.h"
#include "App/IGameMove.h"

#include "View/SpriteRenderer.h"
#include "View/Card.h"
#include "View/Button.h"
#include "View/CardStack.h"

#include "MyGL/Platform/Window2D.h"
#include "MyGL/Events/MouseEvent.h"

#include <MyGL/GLErrors.h>


MouseInput::MouseInput(MyGL::Window2D& win, SpriteRenderer& spriteRenderer, IGameMove& gameMove)
	: m_win(win)
	, m_spriteRenderer(spriteRenderer)
	, m_gameMove(gameMove)
	, m_mousePos(0.0f, 0.0f)
	, m_dragStack(nullptr)
	, m_dragSourceStack(nullptr)
	, m_dragStartPos(0.0f, 0.0f)
	, m_dragCardOffset(0.0f, 0.0f)
	, m_buttonPushed(nullptr)
{
}

bool MouseInput::OnMouseMoved(MyGL::MouseMovedEvent& evt)
{
	glm::vec2 mousePos = glm::vec2(evt.GetX(), evt.GetY());
	m_mousePos = m_win.ConvertFromScreenToProjectionViewCoords(mousePos);

	if (m_dragStack != nullptr)
	{
		m_dragStack->setPosition(m_mousePos - m_dragCardOffset);
	}
	return true;
}

bool MouseInput::OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt)
{
	LOG_INFO("Cards App: Mouse down");

	Sprite* sprite = m_spriteRenderer.HitTest(m_win, m_mousePos);
	if (sprite == nullptr)
		return false;

	switch (sprite->getType())
	{
	case SpriteType::Card:
		return OnCardPressed(*(Card*)sprite);
		break;

	case SpriteType::Button:
		return OnButtonPressed(*(Button*)sprite);
		break;
	}
	return false;
}

bool MouseInput::OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt)
{
	LOG_INFO("Cards App: Mouse up");
	if (m_dragStack != nullptr)
	{
		OnDragStackReleased();
		m_dragStack = nullptr;
		m_dragSourceStack = nullptr;
	}

	if (m_buttonPushed != nullptr)
	{
		OnButtonReleased(*m_buttonPushed);
		m_buttonPushed = nullptr;
	}
	return true;
}

bool MouseInput::OnCardPressed(Card& chosenCard)
{
	CardStack* cardStack = chosenCard.getParentStack();
	assert(cardStack != nullptr);

	Card* moveCard = &chosenCard;

	if (cardStack->getStackType() == CardStack::Type::Slot)
	{
		if (!moveCard->IsDraggable())
		{
			moveCard = cardStack->GetMostDraggable();
			if (moveCard == nullptr)
				return false;
		}
		if (moveCard->IsDraggable())
		{
			m_dragStartPos = m_mousePos;
			m_dragCardOffset = m_mousePos - moveCard->getTopLeft();

			// Take the cards from the source stack into a temporary.
			m_dragSourceStack = moveCard->getParentStack();
			auto movingCards = m_dragSourceStack->RemoveCards(*moveCard);

			// Create and position a "drag Stack"
			m_dragStack = std::make_unique<CardStack>(CardStack::Type::Drag);
			m_dragStack->setPosition(moveCard->getPosition());

			// Move the cards into the Drag Stack.
			for (auto card : movingCards)
			{
				m_dragStack->AddCard(*card);
				card->BringToTop();
			}
			return true;
		}
	}

	if (cardStack->getStackType() == CardStack::Type::Stock)
	{
		m_gameMove.Deal();
	}
	return true;
}


bool MouseInput::OnDragStackReleased()
{
	Sprite* target = m_spriteRenderer.HitTest(m_win, *m_dragStack);

	// The animation takes some time and needs to take ownership of the dragStack.
	m_gameMove.DropDragStack(std::move(m_dragStack), target, *m_dragSourceStack);

	return true;
}


bool MouseInput::OnButtonPressed(Button& button)
{
	std::cout << "HIT button" << std::endl;
	m_buttonPushed = &button;
	return true;
}

bool MouseInput::OnButtonReleased(Button& button)
{
	button.OnClick();
	return true;
}
