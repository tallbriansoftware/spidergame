#pragma once

#include "MyGL/Events/Event.h"

#include <stdint.h>

#include <string>
#include <functional>

#include <glm/glm.hpp>


struct GLFWwindow;

namespace MyGL
{
	enum class WindowResizeBehaviour { NoStretch, Stretch, FillWidth, FillHeight };

	class Window2D
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	private:
		std::string m_title;
		uint32_t m_width;
		uint32_t m_height;
		glm::mat4 m_projection;
		glm::vec2 m_projSize;
		glm::mat4 m_view;
		WindowResizeBehaviour m_resizeBehaviour;
		float m_projWidth;
		float m_projHeight;
		glm::vec3 m_translation;

	protected:
		// Protected constructor.  Only allow derived classes to be created.
		Window2D(const std::string& title, uint32_t width, uint32_t height);

	public:
		~Window2D();

	public:
		virtual void OnUpdate() = 0;
		virtual void OnShutdown() = 0;
		virtual GLFWwindow* GetNativeWindow() = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		void SetCameraView(const glm::vec3& translation);
		glm::mat4 GetViewProjection() const;

		glm::mat4 GetInverse_ViewProjection() const;
		glm::vec2 GetScreenSize() const;

		void SetWindowResizeBehaviour(WindowResizeBehaviour behaviour, float width, float height);
		void SetProjectionForWindowSize(int width, int height);
		void OnResize(int width, int height);
		glm::vec2 ConvertFromScreenToProjectionViewCoords(glm::vec2 pos) const;

		void SetVSync(bool enabled);
		bool IsVSync() const;

	protected:
		void SetWindowProjection(float width, float height);
		glm::vec2 getProjectionSize(float width, float height) const;

	protected:
		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;

	};
}