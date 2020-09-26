#pragma once

#include "Vector2.h"

namespace Math
{
	struct Rectangle
	{
	public:
		Rectangle();
		Rectangle(const Math::Vector2& topLeft, const Math::Vector2& bottomRight);
		~Rectangle();

	public:
		Math::Vector2 topLeft;
		Math::Vector2 bottomRight;
	};
}