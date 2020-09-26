#include "Transform3D.h"

void fix_x_rotation(float& x)
{
	//Fix wierd rotation, same as for camera
	if (x > ((PI_F / 2.0f) - (PI_F / 180.0f)))
		x = (PI_F / 2.0f) - (PI_F / 180.0f);
	else if (x < ((-PI_F / 2.0f) + (PI_F / 180.0f)))
		x = ((-PI_F / 2.0f) + (PI_F / 180.0f));
}

Transform3D::Transform3D()
	: m_Transform(Math::Matrix4::Identity()),
	m_Rotation(),
	m_Position(),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_NeedsUpdate(true)
{
	UpdateMatrix();
}

Transform3D::~Transform3D()
{
}

void Transform3D::Translate(const Math::Vector3& translation)
{
	m_Position += translation;
	m_NeedsUpdate = true;
}

void Transform3D::Rotate(const Math::Vector3& rotation)
{
	m_Rotation += rotation;

	fix_x_rotation(m_Rotation.x);

	m_NeedsUpdate = true;
}

void Transform3D::Scale(const Math::Vector3& scale)
{
	m_Scale.x *= scale.x;
	m_Scale.y *= scale.y;
	m_Scale.z *= scale.z;
	m_NeedsUpdate = true;
}

void Transform3D::Scale(float scale)
{
	m_Scale.x *= scale;
	m_Scale.y *= scale;
	m_Scale.z *= scale;
	m_NeedsUpdate = true;
}

void Transform3D::SetScale(const Math::Vector3& scale)
{
	m_Scale = scale;
	m_NeedsUpdate = true;
}

void Transform3D::SetScale(float scale)
{
	m_Scale.x = scale;
	m_Scale.y = scale;
	m_Scale.z = scale;
	m_NeedsUpdate = true;
}

void Transform3D::SetPosition(const Math::Vector3& position)
{
	m_Position = position;
	m_NeedsUpdate = true;
}

void Transform3D::SetRotation(const Math::Vector3& rotation)
{
	m_Rotation = rotation;

	fix_x_rotation(m_Rotation.x);

	m_NeedsUpdate = true;
}

Math::Matrix4 Transform3D::GetRotationMatrix() const
{
	return Math::Matrix4::Rotation(m_Rotation.x, m_Rotation.y, m_Rotation.z);
}

Math::Matrix4 Transform3D::GetScaleMatrix() const
{
	return Math::Matrix4::Scale(m_Scale);
}

Math::Matrix4 Transform3D::GetTranslationMatrix() const
{
	return Math::Matrix4::Translation(m_Position);
}

void Transform3D::UpdateMatrix() const
{
	using namespace Math;

	if (m_NeedsUpdate)
	{
		m_Transform = Matrix4::Scale(m_Scale) * Matrix4::Rotation(m_Rotation.x, m_Rotation.y, m_Rotation.z)
			* Matrix4::Translation(m_Position);

		m_NeedsUpdate = false;
	}
}