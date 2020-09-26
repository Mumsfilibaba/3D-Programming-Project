#pragma once

#include "..\Math\Matrix4.h"
#include "ITransformable3D.h"

class Transform3D : public ITransformable3D
{
public:
	//Data
	struct Data
	{
		Math::Matrix4 transformMatrix;
		Math::Matrix4 normalMatrix;
	};

public:
	Transform3D();
	~Transform3D();

	void Translate(const Math::Vector3& translation) override;
	void Rotate(const Math::Vector3& rotationRad) override;
	void Scale(const Math::Vector3& scale) override;
	void Scale(float scale) override;
	void SetScale(const Math::Vector3& scale);
	void SetScale(float scale);
	void SetPosition(const Math::Vector3& position);
	void SetRotation(const Math::Vector3& rotationRad);

	Math::Matrix4 GetRotationMatrix() const;
	Math::Matrix4 GetScaleMatrix() const;
	Math::Matrix4 GetTranslationMatrix() const;

	inline const Math::Matrix4& GetTransformMatrix() const
	{
		UpdateMatrix();
		return m_Transform;
	}

	inline const Math::Vector3& GetRotation() const
	{
		return m_Rotation;
	}

	inline const Math::Vector3& GetPosition() const
	{
		return m_Position;
	}

	inline const Math::Vector3& GetScale() const
	{
		return m_Scale;
	}

private:
	void UpdateMatrix() const;

private:
	mutable Math::Matrix4 m_Transform;
	Math::Vector3 m_Rotation;
	Math::Vector3 m_Position;
	Math::Vector3 m_Scale;
	mutable bool m_NeedsUpdate;
};