#include "MyGL/Platform/MsWindows.h"

#include "MyGL/GLErrors.h"

#include "MyGL/Events/ApplicationEvent.h"
#include "MyGL/Events/MouseEvent.h"
#include "MyGL/Events/KeyEvent.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdint.h>

namespace
{
	bool s_GlfwInited = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}
}



MyGL::MsWindows::MsWindows(const std::string& title, uint32_t width, uint32_t height)
	: MyGL::Window2D(title, width, height)
{
	if (!s_GlfwInited)
	{
		int success = glfwInit();
		GLCORE_ASSERT(success, "Could not intialize GLFW!");

		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

		glfwSetErrorCallback(GLFWErrorCallback);
		s_GlfwInited = true;
	}

	m_nativeWindow = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_nativeWindow);

	if (glewInit() != GLEW_OK)
	{
		GLCORE_ASSERT(false, "Could not intialize GLEW!");
	}

	LOG_INFO("OpenGL Info:");
	LOG_INFO("  Vendor: %s", glGetString(GL_VENDOR));
	LOG_INFO("  Renderer: %s", glGetString(GL_RENDERER));
	LOG_INFO("  Version: %s", glGetString(GL_VERSION));

	glfwSetWindowUserPointer(m_nativeWindow, &m_Data);
	SetVSync(true);
}


void MyGL::MsWindows::SetEventCallback(const EventCallbackFn& callback)
{
	m_Data.EventCallback = callback;

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_nativeWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

	glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

	glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetCharCallback(m_nativeWindow, [](GLFWwindow* window, uint32_t keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

	glfwSetMouseButtonCallback(m_nativeWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(m_nativeWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
}


void MyGL::MsWindows::OnShutdown()
{
	glfwDestroyWindow(m_nativeWindow);
}

void MyGL::MsWindows::OnUpdate()
{
	glfwSwapBuffers(m_nativeWindow);
	glfwPollEvents();
}
