#pragma once

#include <string>
//#include "MyGL/Events/Event.h"
//#include "MyGL//Events/ApplicationEvent.h"
//#include "MyGL//Events/MouseEvent.h"

//#include "MyImGui.h"

#include <glm/glm.hpp>


#include <memory>

namespace MyGL
{
	class Window2D;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class MouseMovedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
}


class Application
{
public:
	Application(const std::string& name = "Application", uint32_t width = 640, uint32_t height = 480);
	virtual ~Application();

public:
	virtual bool ShouldContinue();
	virtual void Init();
	virtual void Render();
	virtual void EndFrame();
	virtual void Shutdown();
	virtual void Clear();

protected:
	virtual void OnEvent(MyGL::Event& evt);
	virtual bool OnWindowClose(MyGL::WindowCloseEvent& evt);
	virtual bool OnWindowResize(MyGL::WindowResizeEvent& evt);
	virtual bool OnMousedMoved(MyGL::MouseMovedEvent& evt);
	virtual bool OnMouseButtonPressed(MyGL::MouseButtonPressedEvent& evt);
	virtual bool OnMouseButtonReleased(MyGL::MouseButtonReleasedEvent& evt);

protected:
	bool m_running;
	uint32_t m_width;
	uint32_t m_height;
	glm::u32vec2 m_screenSize;
	glm::vec3 m_backgroundColor;
	std::unique_ptr<MyGL::Window2D> m_window2D;

};
