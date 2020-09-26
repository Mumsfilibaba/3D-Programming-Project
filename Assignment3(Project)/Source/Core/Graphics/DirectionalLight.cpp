#include "DirectionalLight.h"

namespace Graphics
{
	DirectionalLight::DirectionalLight()
		: m_NeedsUpdate(true),
		m_Direction(),
		m_Rotation(),
		m_RotationMatrix(1.0f),
		m_View(1.0f),
		m_Projection(1.0f),
		m_Final(1.0f),
		m_Size(10.0f, 10.0f),
		m_FarPlane(20.0f),
		m_NearPlane(1.0f)
	{
		UpdateMatrices();
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	bool DirectionalLight::UpdateMatrices()
	{
		using namespace Math;

		if (m_NeedsUpdate)
		{
			//Set direction
			m_RotationMatrix = Matrix3::Rotation(m_Rotation.z, m_Rotation.x, m_Rotation.y);
			m_Direction = (-Vector3::UP) * m_RotationMatrix;

			//Set up and position
			Vector3 up = Vector3::FORWARD * m_RotationMatrix;
			Vector3 position = (-m_Direction) * ((m_FarPlane - m_NearPlane) / 2.0f);
			float x = m_Size.x / 2.0f;
			float y = m_Size.y / 2.0f;

			//Set matrices
			m_View = Matrix4::LookAt(up, position + m_Direction, position);
			m_Projection = Matrix4::Orthographic(-x, x, y, -y, m_FarPlane, m_NearPlane);
			m_Final = m_View * m_Projection;

			//Normalize direction
			m_Direction.Normalize();
			//Set flag to false
			m_NeedsUpdate = false;

			return true;
		}

		return false;
	}

	void DirectionalLight::SetColor(const ColorF & color)
	{
		m_Color = color;
	}

	void DirectionalLight::SetIntensity(float intensity)
	{
		m_Intensity = intensity;
	}

	void DirectionalLight::SetIBLIntensity(float iblIntensity)
	{
		m_IblIntensity = iblIntensity;
	}

	void DirectionalLight::SetViewport(float x, float y)
	{
		m_Size = Math::Vector2(x, y);
		m_NeedsUpdate = true;
	}

	void DirectionalLight::SetNearPlane(float nearPlane)
	{
		m_NearPlane = nearPlane;
		m_NeedsUpdate = true;
	}

	void DirectionalLight::SetFarPlane(float farPlane)
	{
		m_FarPlane = farPlane;
		m_NeedsUpdate = true;
	}

	void DirectionalLight::SetRotation(const Math::Vector3& rotation)
	{
		m_Rotation = rotation;
		m_NeedsUpdate = true;
	}
}