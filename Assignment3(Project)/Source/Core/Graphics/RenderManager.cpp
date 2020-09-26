#include "RenderManager.h"

namespace Graphics
{
	RenderSettings::RenderSettings()
		: m_Resolution(),
		m_ShadowRes()
	{
	}

	RenderSettings::~RenderSettings()
	{
	}

	void RenderSettings::CalculateViewportSize(const Math::Point& targetSize, Viewport& viewport) const
	{
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		if (((float)targetSize.x / (float)targetSize.y) < GetCurrentAspectRatio())
		{
			x = 0;
			w = (float)targetSize.x;

			h = ((1.0f / GetCurrentAspectRatio()) * targetSize.x);
			y = (targetSize.y / 2.0f) - (h / 2.0f);
		}
		else
		{
			y = 0;
			h = (float)targetSize.y;

			w = (GetCurrentAspectRatio() * targetSize.y);
			x = (targetSize.x / 2.0f) - (w / 2.0f);
		}

		viewport.width = (int32)w;
		viewport.height = (int32)h;
		viewport.x = (int32)x;
		viewport.y = (int32)y;
	}
}