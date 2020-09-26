#include "MathUtilities.h"

namespace Math
{
	double to_degrees(double radians)
	{
		return (180.0 * radians) / PI;
	}

	double to_radians(double degrees)
	{
		return (PI * degrees) / 180.0;
	}

	float to_degrees_f(float radians)
	{
		return (180.0f * radians) / PI_F;
	}

	float to_radians_f(float degrees)
	{
		return (PI_F * degrees) / 180.0f;
	}
}