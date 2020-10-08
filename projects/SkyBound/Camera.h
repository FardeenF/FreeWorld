#pragma once

#include "Entity.h"


class Camera
{
public:

	static Entity* current;

	Camera(Entity& owner);
	virtual ~Camera();

	void Update();

	
	const glm::mat4& GetVP();

	const glm::mat4& GetView();
	const glm::mat4& GetProj();

	void Ortho(float left, float right, float bottom, float top, float near, float far);

	void Perspective(float fovYDegrees, float aspect, float near, float far);

protected:

	Entity* m_owner;
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_viewProjection;
};
