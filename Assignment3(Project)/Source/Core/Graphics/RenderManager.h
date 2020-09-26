#pragma once

#include "..\Math\Point.h"

namespace Graphics
{
	struct Viewport;

	class RenderSettings
	{
	public:
		RenderSettings();
		~RenderSettings();

		void CalculateViewportSize(const Math::Point& targetSize, Viewport& viewport) const;
		
		inline void SetResolution(uint16 width, uint16 height)
		{
			SetResolution(Math::Point(width, height));
		}

		inline void SetResolution(const Math::Point& size)
		{
			m_Resolution = size;
		}

		inline void SetShadowResolution(uint16 width, uint16 height)
		{
			SetShadowResolution(Math::Point(width, height));
		}

		inline void SetShadowResolution(const Math::Point& size)
		{
			m_ShadowRes = size;
		}
		
		inline const Math::Point& GetResolution() const
		{
			return m_Resolution;
		}

		inline const Math::Point& GetShadowResolution() const
		{
			return m_ShadowRes;
		}
		
		inline float GetCurrentAspectRatio() const
		{
			return (float)m_Resolution.x / m_Resolution.y;
		}

	private:
		Math::Point m_Resolution;
		Math::Point m_ShadowRes;
	};
}