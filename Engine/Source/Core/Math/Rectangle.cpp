#include "Rectangle.h"

namespace Math
{
	Rectangle::Rectangle()
		: topLeft(),
		bottomRight()
	{
	}

	Rectangle::Rectangle(const Math::Vector2& topLeft, const Math::Vector2& bottomRight)
		: topLeft(topLeft),
		bottomRight(bottomRight)
	{
	}

	Rectangle::~Rectangle()
	{
	}
}