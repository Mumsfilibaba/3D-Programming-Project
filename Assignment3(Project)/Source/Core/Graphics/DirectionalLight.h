#pragma once

#include "..\Math\Matrix3.h"
#include "..\Math\Matrix4.h"
#include "..\Graphics\ColorF.h"

namespace Graphics
{
	class DirectionalLight
	{
	public:
		struct Data
		{
		public:
			Data() {}
			~Data() {}

		public:
			Math::Matrix4 lightMatrix;
			Math::Vector3 direction;
			float intensity;
			
			union 
			{
				ColorF color;

				struct
				{
					Math::Vector3 rgb;
					float iblIntensity;
				};
			};
		};

	public:
		DirectionalLight();
		~DirectionalLight();

		bool UpdateMatrices();

		void SetColor(const ColorF& color);
		void SetIntensity(float intensity);
		void SetIBLIntensity(float iblIntensity);

		void SetViewport(float x, float y);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetRotation(const Math::Vector3& position);

		inline const Math::Vector3& GetRotation() const
		{
			return m_Rotation;
		}

		inline float GetFarPlane() const
		{
			return m_FarPlane;
		}

		inline float GetNearPlane() const
		{
			return m_NearPlane;
		}

		inline const Math::Vector2& GetSize() const
		{
			return m_Size;
		}

		inline const Math::Vector3& GetDirection() const
		{
			return m_Direction;
		}

		inline const Math::Matrix4& GetViewMatrix() const
		{
			return m_View;
		}

		inline const Math::Matrix4& GetProjectionMatrix() const
		{
			return m_Projection;
		}

		inline const Math::Matrix4& GetMatrix() const
		{
			return m_Final;
		}

		inline const ColorF& GetColor() const
		{
			return m_Color;
		}

		inline float GetIntensity() const
		{
			return m_Intensity;
		}

		inline float GetIBLIntensity() const
		{
			return m_IblIntensity;
		}

	private:
		Math::Matrix4 m_Final;
		Math::Matrix4 m_Projection;
		Math::Matrix4 m_View;
		Math::Vector3 m_Direction;
		Math::Vector3 m_Rotation;
		Math::Matrix3 m_RotationMatrix;
		Math::Vector2 m_Size;
		ColorF m_Color;
		float m_Intensity;
		float m_IblIntensity;
		float m_NearPlane;
		float m_FarPlane;
		bool m_NeedsUpdate;
	};
}