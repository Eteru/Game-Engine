
#include "camera.h"

Camera::Camera()
: m_position(glm::vec3(0, 0, -3)), m_forward(glm::vec3(0, 0, 1)), m_up(glm::vec3(0, 1, 0)),
	m_fov(70.f), m_ratio(1.33f), m_zNear(0.01f), m_zFar(1000.f), m_camera_speed(0.005f), m_mouse_sensitivity(0.001f)
{
	m_perspective = glm::perspective(m_fov, m_ratio, m_zNear, m_zFar);

	InitPlanes();
}
Camera::Camera(const glm::vec3 & pos, const glm::vec3 & forward, const glm::vec3 & up, float fov, float ratio, float zNear, float zFar, float camera_speed, float sensitivity)
: m_position(pos), m_forward(glm::normalize(forward)), m_up(up), m_fov(fov), m_ratio(ratio),
	m_zNear(zNear),	m_zFar(zFar), m_camera_speed(camera_speed), m_mouse_sensitivity(sensitivity)
{
	m_perspective = glm::perspective(fov, ratio, zNear, zFar);

	InitPlanes();
}

void Camera::InitPlanes()
{
	m_hNear = 2.f * tan(m_fov * .5f) * m_zNear;
	m_wNear = m_hNear * m_ratio;

	m_hFar = 2.f * tan(m_fov * .5f) * m_zFar;
	m_wFar = m_hFar * m_ratio;
}

Frustrum & Camera::GetFrustrum()
{
	glm::vec3 right = glm::normalize(glm::cross(m_forward, m_up));

	glm::vec3 fc = m_position + m_forward * m_zFar;
	glm::vec3 fvDist = m_up * m_hFar * .5f;
	glm::vec3 fhDist = right * m_wFar * .5f;

	glm::vec3 ftl = fc + fvDist - fhDist;
	glm::vec3 ftr = fc + fvDist + fhDist;
	glm::vec3 fbl = fc - fvDist - fhDist;
	glm::vec3 fbr = fc - fvDist + fhDist;

	glm::vec3 nc = m_position + m_forward * m_zNear;
	glm::vec3 nvDist = m_up * m_hNear * .5f;
	glm::vec3 nhDist = right * m_wNear * .5f;

	glm::vec3 ntl = nc + nvDist - nhDist;
	glm::vec3 ntr = nc + nvDist + nhDist;
	glm::vec3 nbl = nc - nvDist - nhDist;
	glm::vec3 nbr = nc - nvDist + nhDist;

	m_frustrum.SetPlanes(ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr);

	return m_frustrum;
}

void Camera::MoveForward(uint32_t count)
{
	{
		glm::vec3 new_pos = m_position + count * m_camera_speed * m_forward;

		if (new_pos.x < 0) {
			new_pos.x = 0;
		}
		if (new_pos.y < 0) {
			new_pos.y = 0;
		}
		if (new_pos.z < 0) {
			new_pos.z = 0;
		}

		m_position = new_pos;
	}
}

void Camera::MoveBackwards(uint32_t count)
{
	glm::vec3 new_pos = m_position - count * m_camera_speed * m_forward;

	if (new_pos.x < 0) {
		new_pos.x = 0;
	}
	if (new_pos.y < 0) {
		new_pos.y = 0;
	}
	if (new_pos.z < 0) {
		new_pos.z = 0;
	}

	m_position = new_pos;
}

void Camera::MoveLeft(uint32_t count)
{
	glm::vec3 new_pos = m_position - glm::normalize(glm::cross(m_forward, m_up)) * (count * m_camera_speed);

	if (new_pos.x < 0) {
		new_pos.x = 0;
	}
	if (new_pos.y < 0) {
		new_pos.y = 0;
	}
	if (new_pos.z < 0) {
		new_pos.z = 0;
	}

	m_position = new_pos;
}

void Camera::MoveRight(uint32_t count)
{
	glm::vec3 new_pos = m_position + glm::normalize(glm::cross(m_forward, m_up)) * (count * m_camera_speed);

	if (new_pos.x < 0) {
		new_pos.x = 0;
	}
	if (new_pos.y < 0) {
		new_pos.y = 0;
	}
	if (new_pos.z < 0) {
		new_pos.z = 0;
	}

	m_position = new_pos;
}

void Camera::Rotate(int xrel, int yrel)
{
	if (xrel != 0)
		m_forward = glm::rotate(m_forward, m_mouse_sensitivity * -xrel, glm::vec3(0.f, 1.f, 0.f));

	if (yrel != 0)
		m_forward = glm::rotate(m_forward, m_mouse_sensitivity * -yrel, glm::vec3(1.f, 0.f, 0.f));

	m_forward = glm::normalize(m_forward);
}
