#pragma once

#include "..\Math\Vector3.h"

class ITransformable3D
{
public:
	virtual ~ITransformable3D() {}

	virtual void Translate(const Math::Vector3& translation) = 0;
	virtual void Rotate(const Math::Vector3& rotationRad) = 0;
	virtual void Scale(const Math::Vector3& scale) = 0;
	virtual void Scale(float scale) = 0;
};