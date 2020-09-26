#pragma once

#include "Camera.h"
#include "..\Physics\ITransformable3D.h"

namespace Graphics
{
	class Camera3D : public Camera, public ITransformable3D
	{
	public:
		Camera3D();
		virtual ~Camera3D() {}

		void Translate(const Math::Vector3& translation) override;
		void Rotate(const Math::Vector3& rotationRad) override;
		
		//Scale does not affect the camera
		void Scale(const Math::Vector3& scale) override {}
		void Scale(float scale) override {}

		void SetFov(float fovRad);

		inline float GetFov() const
		{
			return m_Fov;
		}

	protected:
		Math::Matrix4 CalculateProjection() override;

	private:
		float m_Fov;
	};
}