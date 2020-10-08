#include "Camera.h"

#include "GLM/gtx/transform.hpp"


Entity* Camera::current = nullptr;

Camera::Camera(Entity& owner)
{
	
	if (current == nullptr)
		current = &owner;

	m_owner = &owner;

	
	m_projection = glm::mat4(1.0f);
	m_viewProjection = glm::mat4(1.0f);
}

Camera::~Camera()
{
	
	if (current == m_owner)
		current = nullptr;
}

void Camera::Update()
{
	m_view = glm::inverse(m_owner->transform.RecomputeGlobal());
	m_viewProjection = m_projection * m_view;
}

const glm::mat4& Camera::GetVP()
{
	return m_viewProjection;
}

const glm::mat4& Camera::GetView()
{
	return m_view;
}

const glm::mat4& Camera::GetProj()
{
	return m_projection;
}

void Camera::Ortho(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
	Update();
}

void Camera::Perspective(float fovYDegrees, float aspect, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fovYDegrees), aspect, near, far);
	Update();
}
