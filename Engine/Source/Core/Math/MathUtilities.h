#ifndef MATHHELPER_H
#define MATHHELPER_H

#define PI 3.1415926535898
#define E 2.7182818284590
#define PI_F 3.141592f
#define E_f 2.718281f
#define PIx2 2.0 * PI
#define PIx2_F 2.0f * PI_F

#include <math.h>

namespace Math
{
	double to_degrees(double degrees);
	double to_radians(double radians);

	float to_degrees_f(float degrees);
	float to_radians_f(float radians);
}
#endif