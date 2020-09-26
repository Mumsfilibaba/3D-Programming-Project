#include "Camera3D.h"

namespace Graphics
{
	Camera3D::Camera3D() 
		: m_Fov(PI_F / 4)
	{
	}

	void Camera3D::Translate(const Math::Vector3& translation)
	{
		SetPosition(GetPosition() + translation);
	}

	void Camera3D::Rotate(const Math::Vector3& rotationRad)
	{
		SetRotation(GetRotation() + rotationRad);
	}

	Math::Matrix4 Camera3D::CalculateProjection() 
	{
		return Math::Matrix4::Perspective(m_Fov, GetAspectRatio(), GetFarPlane(), GetNearPlane());
	}

	void Camera3D::SetFov(float fovRad) 
	{
		m_Fov = fovRad;
	}
}