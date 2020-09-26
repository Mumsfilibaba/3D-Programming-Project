#pragma once

#include "Camera.h"

namespace Graphics
{
	class Camera2D : public Camera
	{
	public:
		Camera2D();
		virtual ~Camera2D() {}

	protected:
		Math::Matrix4 CalculateProjection() override;
	};
}