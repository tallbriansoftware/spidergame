#include "MyGL/Platform/Window2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <MyGL/GLErrors.h>


MyGL::Window2D::Window2D(const std::string& title, uint32_t width, uint32_t height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    m_Data.Title = title;
    m_Data.Width = width;
    m_Data.Height = height;
    m_Data.VSync = false;
    SetProjectionForWindowSize(width, height);
}


MyGL::Window2D::~Window2D()
{
}


void MyGL::Window2D::SetCameraView(const glm::vec3& translation)
{
    m_translation = translation;
    m_view = glm::translate(glm::mat4(1.0f), translation);
}


glm::mat4 MyGL::Window2D::GetViewProjection() const
{
    return m_projection * m_view;
}


glm::mat4 MyGL::Window2D::GetInverse_ViewProjection() const
{
    auto aspect = getProjectionSize((float)m_width, (float)m_height);

    glm::mat4 inv_proj = glm::ortho(0.0f, (float)m_width/(aspect.x/2),
                                          -(float)m_height/(aspect.y/2),
                                            0.0f, -1.0f, 1.0f);
    return inv_proj;
}

glm::vec2 MyGL::Window2D::GetScreenSize() const
{
    return glm::vec2((float)m_width, (float)m_height);
}

glm::vec2 MyGL::Window2D::ConvertFromScreenToProjectionViewCoords(glm::vec2 pos) const
{
    glm::vec4 pos4 = glm::vec4(pos, 0, 0);
    glm::mat4 inv_proj = GetInverse_ViewProjection();
    glm::vec4 pv_coords = inv_proj * pos4;

    return pv_coords;
}


void MyGL::Window2D::SetWindowResizeBehaviour(WindowResizeBehaviour behaviour, float width, float height)
{
    m_resizeBehaviour = behaviour;
    m_projWidth = width;
    m_projHeight = height;
}


glm::vec2 MyGL::Window2D::getProjectionSize(float width, float height) const
{
    switch (m_resizeBehaviour)
    {
    case WindowResizeBehaviour::NoStretch:
        return glm::vec2(width, height);

    case WindowResizeBehaviour::Stretch:
        return glm::vec2(m_projWidth, m_projHeight);

    case WindowResizeBehaviour::FillHeight:
        return glm::vec2(m_projHeight * (float)width / height, m_projHeight);

    case WindowResizeBehaviour::FillWidth:
        return glm::vec2(m_projWidth, m_projWidth * (float)height / width);
    }
    return glm::vec2(0.0f);
}

void MyGL::Window2D::SetProjectionForWindowSize(int width, int height)
{
    m_width = width;
    m_height = height;

    auto aspect = getProjectionSize((float)width, (float)height);
    SetWindowProjection(aspect.x, aspect.y);
}

void MyGL::Window2D::OnResize(int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
    SetProjectionForWindowSize(width, height);
}


void MyGL::Window2D::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

bool MyGL::Window2D::IsVSync() const
{
    return m_Data.VSync;
}


//  Set the Projection Matrix.
//     Always orthographic 
//     Always with the origin in the upper left corner.
void MyGL::Window2D::SetWindowProjection(float width, float height)
{
    m_projSize = glm::vec2(width, height);

    m_projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
}

