#include "App/Application.h"

#include "MyGL/Events/Event.h"
#include "MyGL//Events/ApplicationEvent.h"
#include "MyGL//Events/MouseEvent.h"

#include "MyGL/Platform/Window2D.h"
#include "MyGL/Platform/MsWindows.h"

#include "MyGL/GLErrors.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stdint.h"

#include <memory>


Application::Application(const std::string& name, uint32_t width, uint32_t height)
	: m_running(true)
	, m_width(width)
	, m_height(height)
	, m_screenSize(width, height)
	, m_backgroundColor({0.2f, 0.2f, 0.2f})
{
}

Application::~Application()
{
}

bool Application::ShouldContinue()
{
	return m_running;
}


void Application::Init()
{
	m_window2D = std::make_unique<MyGL::MsWindows>(
		"OpenGL-Application", m_width, m_height);

	// Defaults, can be overridden in the derived class.
	m_window2D->SetWindowResizeBehaviour(MyGL::WindowResizeBehaviour::NoStretch, (float)m_width, (float)m_height);
	m_window2D->SetProjectionForWindowSize(m_width, m_height);

	using namespace std::placeholders;
	m_window2D->SetEventCallback([this](MyGL::Event& evt) {this->OnEvent(evt); });


	// Set Alpha Blend Mode
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Application::Render()
{
}

void Application::EndFrame()
{
	// process IO Events and Swap Buffers
	m_window2D->OnUpdate();
}

void Application::Shutdown()
{
	m_window2D->OnShutdown();
	exit(0);
}

void Application::Clear()
{
	GLCall(glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Application::OnEvent(MyGL::Event& evt)
{
	MyGL::EventDispatcher dispatcher(evt);

	//std::cout << "Event:  " << evt.ToString() << std::endl;
	switch (evt.GetEventType())
	{
	case MyGL::EventType::WindowClose:
		evt.Handled = OnWindowClose(static_cast<MyGL::WindowCloseEvent&>(evt));
		break;

	case MyGL::EventType::WindowResize:
		evt.Handled = OnWindowResize(static_cast<MyGL::WindowResizeEvent&>(evt));
		break;

	case MyGL::EventType::MouseButtonPressed:
		evt.Handled = OnMouseButtonPressed(static_cast<MyGL::MouseButtonPressedEvent&>(evt));
		break;

	case MyGL::EventType::MouseButtonReleased:
		evt.Handled = OnMouseButtonReleased(static_cast<MyGL::MouseButtonReleasedEvent&>(evt));
		break;

	case MyGL::EventType::MouseMoved:
		evt.Handled = OnMousedMoved(static_cast<MyGL::MouseMovedEvent&>(evt));

	}
}

bool Application::OnWindowClose(MyGL::WindowCloseEvent& evt)
{
	m_running = false;
	return true;
}

bool Application::OnWindowResize(MyGL::WindowResizeEvent& evt)
{
	m_screenSize = glm::u32vec2(evt.GetWidth(), evt.GetHeight());
	m_window2D->OnResize(m_screenSize.x, m_screenSize.y);

	// calling Render here allows us to paint through the resize events.
	this->Clear();

	this->Render();

	this->EndFrame();

	return true;
}

bool Application::OnMousedMoved(MyGL::MouseMovedEvent& evt)
{
	return false;
}

bool Application::OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt)
{
	return false;
}

bool Application::OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt)
{
	return false;
}

