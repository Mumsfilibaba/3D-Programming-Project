#include "Camera2D.h"

Graphics::Camera2D::Camera2D()
{
}

Math::Matrix4 Graphics::Camera2D::CalculateProjection()
{
	return Math::Matrix4::Orthographic(GetAspectRatio(), GetFarPlane(), GetNearPlane());
}