#include "App/AppCards.h"

#include "App/MouseInput.h"
#include "App/Dealing.h"
#include "App/SpiderGame.h"
#include "App/GameResources.h"

#include "Animation/SpiderAnimator.h"
#include "View/Button.h"
#include "View/Card.h"
#include "Model/CardEnums.h"
#include "Model/CardModel.h"
#include "View/CardStack.h"
#include "View/SpriteFactory.h"
#include "View/SpriteRenderer.h"
#include "View/StackSlot.h"

#include "Animate/Animator.h"
#include "Utils/GameTime.h"

#include "MyGL/Platform/Window2D.h"
#include "MyGL/Events/MouseEvent.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

#include <MyGL/GLErrors.h>


namespace{
	int c_buttonLeftEdge = 30;
	int c_buttonStackTop = 600;
	float c_buttonHeight = 100.0f;
	float c_buttonWidth = 100.0f;
	float c_buttonVerticalSpacingFactor = 1.3f;
}


AppCards::AppCards(const std::string& name, uint32_t width, uint32_t height)
	: Application(name, width, height)
{
	// Nice Green
	m_backgroundColor = glm::vec3(0.1f, 0.5f, 0.0f);
}

AppCards::~AppCards()
{
}


void AppCards::Init()
{
	Application::Init();

	m_window2D->SetWindowResizeBehaviour(MyGL::WindowResizeBehaviour::FillWidth, 2700, 1350);
	m_window2D->SetProjectionForWindowSize(m_width, m_height);

	m_gameResources = std::make_unique<GameResources>();
	m_spriteRenderer = std::make_unique<SpriteRenderer>();
	m_spriteFactory = std::make_unique<SpriteFactory>(*m_spriteRenderer, *m_gameResources);
	m_animator = std::make_unique<Animator>();
	m_spiderAnimator = std::make_unique<SpiderAnimator>(*m_animator);
	m_spiderGame = std::make_unique<SpiderGame>(*m_spriteFactory, *m_spriteRenderer, *m_spiderAnimator);

	m_mouseInput = std::make_unique<MouseInput>(*m_window2D, *m_spriteRenderer, *m_spiderGame);
	m_gameTime = std::make_unique<GameTime>();

	auto undoButton = m_spriteFactory->CreateButton("Undo");
	undoButton->setPosition({ c_buttonLeftEdge, c_buttonStackTop, 0 });
	undoButton->setSize({ c_buttonWidth, c_buttonHeight });
	undoButton->setOnClick([this]() { this->OnUndoButtonClicked(); });
	m_spriteRenderer->AddButton(undoButton);

	auto test1Button = m_spriteFactory->CreateButton("Test1");
	test1Button->setPosition({ c_buttonLeftEdge, c_buttonStackTop
		+ 1 * c_buttonVerticalSpacingFactor * c_buttonHeight, 0 });
	test1Button->setSize({ c_buttonWidth, c_buttonHeight });
	test1Button->setOnClick([this]() { this->OnTest1ButtonClicked(); });
	m_spriteRenderer->AddButton(test1Button);

	auto newButton = m_spriteFactory->CreateButton("NewGame");
	newButton->setPosition({ c_buttonLeftEdge, c_buttonStackTop
		+ 2 * c_buttonVerticalSpacingFactor * c_buttonHeight, 0 });
	newButton->setSize({ c_buttonWidth, c_buttonHeight });
	newButton->setOnClick([this]() { this->OnNewGameButtonClicked(); });
	m_spriteRenderer->AddButton(newButton);
}


void AppCards::Render()
{
	Application::Render();
	m_spriteRenderer->DrawSprites(*m_window2D);

	m_animator->Render(m_gameTime->Milliseconds());
}


bool AppCards::OnMousedMoved(MyGL::MouseMovedEvent& evt)
{
	return m_mouseInput->OnMouseMoved(evt);
}

bool AppCards::OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt)
{
	return m_mouseInput->OnMouseButtonPressed(evt);
}

bool AppCards::OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt)
{
	return m_mouseInput->OnMouseButtonReleased(evt);
}


void AppCards::OnUndoButtonClicked()
{
	std::cout << "Undo pressed" << std::endl;
	m_spiderGame->Undo();
}

void AppCards::OnTest1ButtonClicked()
{
	std::cout << "Test 1 pressed" << std::endl;
	m_spiderGame->SetTestConfig(1);
}

void AppCards::OnNewGameButtonClicked()
{
	std::cout << "New Game pressed" << std::endl;
	m_spiderGame->Reshuffle();
}
