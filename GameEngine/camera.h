#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#include "Frustrum.h"

#define ANG2RAD 3.14159265358979323846/180.0

class Camera
{
public:
	Camera();

	Camera(const glm::vec3 & pos, const glm::vec3 & forward,
		const glm::vec3 & up, float fov, float ratio, float zNear, float zFar,
		float camera_speed, float sensitivity);

	virtual ~Camera()
	{
	}
	
	inline glm::vec3 GetPosition(void) const
	{
		return m_position;
	}

	inline glm::vec3 GetForward(void) const
	{
		return m_forward;
	}

	inline glm::vec3 GetUp(void) const
	{
		return m_up;
	}

	inline glm::mat4 GetViewProjection(void) const
	{
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::mat4 GetViewMatrix(void) const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::mat4 GetProjectionMatrix(void) const
	{
		return m_perspective;
	}

	inline float GetMouseSensitivity(void) const
	{
		return m_mouse_sensitivity;
	}

	inline float GetCameraSpeed(void) const
	{
		return m_camera_speed;
	}

	inline float GetFOV(void) const
	{
		return m_fov;
	}

	inline float GetNearPlane(void) const
	{
		return m_zNear;
	}

	inline float GetFarPlane(void) const
	{
		return m_zFar;
	}


	void InitPlanes();

	Frustrum & GetFrustrum();

	void MoveForward(uint32_t count);
	void MoveBackwards(uint32_t count);
	void MoveLeft(uint32_t count);
	void MoveRight(uint32_t count);
	void Rotate(int xrel, int yrel);

private:
	float m_camera_speed;
	float m_mouse_sensitivity;
	float m_fov;
	float m_ratio;
	float m_zNear;
	float m_zFar;
	float m_hFar;
	float m_wFar;
	float m_hNear;
	float m_wNear;

	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;

	Frustrum m_frustrum;
};

