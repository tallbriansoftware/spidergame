#pragma once

#include "MyGL/Platform/Window2D.h"

#include <string>

#include <stdint.h>


namespace MyGL
{
	class MsWindows : public MyGL::Window2D
	{
	public:
		MsWindows(const std::string& title, uint32_t width, uint32_t height);
		void OnUpdate() override;
		void OnShutdown() override;
		GLFWwindow* GetNativeWindow() override { return m_nativeWindow;  }

		void SetEventCallback(const EventCallbackFn& callback);

	private:

	private:
		GLFWwindow* m_nativeWindow;

	};
}