#pragma once

#include "..\Math\Matrix3.h"
#include "..\Math\Matrix4.h"
#include "..\..\Defines.h"

namespace Graphics
{
	class Camera
	{
	public:
		struct Data
		{
			Math::Matrix4 matrix;
			Math::Matrix4 rotationMatrix;
			Math::Vector4 position;
		};

	public:
		Camera();
		virtual ~Camera() {}

		bool UpdateMatrices();

		void SetPosition(const Math::Vector3& position);
		void SetRotation(const Math::Vector3& rotation);
		void SetRotation(float x, float y, float z);
		void SetFarPlane(float farZ);
		void SetNearPlane(float nearZ);
		void SetAspectRatio(float aspectWiHe);

		inline float GetFarPlane() const
		{
			return m_FarZ;
		}

		inline float GetNearPlane() const
		{
			return m_NearZ;
		}

		inline float GetAspectRatio() const
		{
			return m_AspectWiHe;
		}

		inline const Math::Vector3& GetLookAt() const
		{
			return m_LookAt;
		}

		inline const Math::Vector3& GetPosition() const
		{
			return m_Position;
		}

		inline const Math::Vector3& GetRotation() const
		{
			return m_Rotation;
		}

		inline const Math::Matrix3& GetRotationMatrix() const
		{
			return m_RotationMatrix;
		}

		inline const Math::Matrix4& GetProjection() const
		{
			return m_Projection;
		}

		inline const Math::Matrix4& GetView() const
		{
			return m_View;
		}

		inline const Math::Matrix4& GetMatrix() const
		{
			return m_Final;
		}

	protected:
		virtual Math::Matrix4 CalculateProjection() = 0;

	private:
		Math::Matrix4 m_Final;
		Math::Matrix4 m_View;
		Math::Matrix4 m_Projection;
		Math::Matrix3 m_RotationMatrix;

		Math::Vector3 m_Position;
		Math::Vector3 m_Rotation;
		Math::Vector3 m_LookAt;
		float m_FarZ;
		float m_NearZ;
		float m_AspectWiHe;
		bool m_NeedsUpdate;
	};
}