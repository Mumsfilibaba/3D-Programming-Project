#include "Camera.h"

#include <iostream>

namespace Graphics
{
	Camera::Camera()
		: m_Position(0.0f, 0.0f, -1.0f),
		m_Rotation(),
		m_LookAt(),
		m_Final(Math::Matrix4::Identity()),
		m_View(Math::Matrix4::Identity()),
		m_Projection(Math::Matrix4::Identity()),
		m_RotationMatrix(Math::Matrix3::Identity()),
		m_FarZ(1000.0f),
		m_NearZ(0.1f),
		m_NeedsUpdate(false)
	{
	}

	bool Camera::UpdateMatrices()
	{
		using namespace Math;

		if (m_NeedsUpdate)
		{
			m_LookAt = m_Position + (Vector3::FORWARD * m_RotationMatrix);

			m_View = Matrix4::LookAt(Vector3::UP * m_RotationMatrix, m_LookAt, m_Position);
			m_Projection = CalculateProjection();

			m_Final = m_View * m_Projection;

			m_NeedsUpdate = false;
			return true;
		}

		return false;
	}

	void Camera::SetPosition(const Math::Vector3& position)
	{
		m_Position = position;

		m_NeedsUpdate = true;
	}

	void Camera::SetRotation(const Math::Vector3& rotation)
	{
		SetRotation(rotation.x, rotation.y, rotation.z);
	}

	void Camera::SetRotation(float x, float y, float z)
	{
		using namespace Math;

		if (x > ((PI_F / 2.0f) - (PI_F / 180.0f)))
			m_Rotation.x = (PI_F / 2.0f) - (PI_F / 180.0f);
		else if (x < ((-PI_F / 2.0f) + (PI_F / 180.0f)))
			m_Rotation.x = ((-PI_F / 2.0f) + (PI_F / 180.0f));
		else
			m_Rotation.x = x;

		m_Rotation.y = y;
		m_Rotation.z = z;

		m_RotationMatrix = Matrix3::Rotation(m_Rotation.z, m_Rotation.x, m_Rotation.y);

		m_NeedsUpdate = true;
	}

	void Camera::SetFarPlane(float farZ)
	{
		m_FarZ = farZ;

		m_NeedsUpdate = true;
	}

	void Camera::SetNearPlane(float nearZ)
	{
		m_NearZ = nearZ;

		m_NeedsUpdate = true;
	}

	void Camera::SetAspectRatio(float aspectWiHe)
	{
		if (aspectWiHe == m_AspectWiHe)
			return;

		m_AspectWiHe = aspectWiHe;

		m_NeedsUpdate = true;
	}
}