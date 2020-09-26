#include <cmath>
#include "ColorF.h"

const ColorF ColorF::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const ColorF ColorF::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const ColorF ColorF::GRAY(0.502f, 0.502f, 0.502f, 1.0f);
const ColorF ColorF::DARKGRAY(0.251f, 0.251f, 0.251f, 1.0f);
const ColorF ColorF::SOFTGRAY(0.753f, 0.753f, 0.753f, 1.0f);
const ColorF ColorF::RED(1.0f, 0.0f, 0.0f, 1.0f);
const ColorF ColorF::DARKRED(0.502f, 0.0f, 0.0f, 1.0f);
const ColorF ColorF::SOFTRED(1.0f, 0.251f, 0.251f, 1.0f);
const ColorF ColorF::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const ColorF ColorF::SOFTGREEN(0.251f, 1.0f, 0.251f, 1.0f);
const ColorF ColorF::DARKGREEN(0.0f, 0.502f, 0.0f, 1.0f);
const ColorF ColorF::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const ColorF ColorF::DARKBLUE(0.0f, 0.0f, 0.502f, 1.0f);
const ColorF ColorF::SOFTBLUE(0.325f, 0.325f, 1.0f, 1.0f);
const ColorF ColorF::CORNFLOWERBLUE(0.392f, 0.584f, 0.929f, 1.0f);
const ColorF ColorF::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const ColorF ColorF::SOFTYELLOW(1.0f, 1.0f, 0.251f, 1.0f);
const ColorF ColorF::DARKYELLOW(0.502f, 0.502f, 0.0f, 1.0f);
const ColorF ColorF::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
const ColorF ColorF::PURPLE(0.502f, 0.0f, 0.502f, 1.0f);
const ColorF ColorF::PINK(1.0f, 0.753f, 0.796f, 1.0f);



ColorF::ColorF(float r, float g, float b, float a)
	: r(fabs(r)),
	g(fabs(g)),
	b(fabs(b)), 
	a(fabs(a))
{
}

ColorF::ColorF(const ColorF& other)
	: r(0.0f),
	g(0.0f),
	b(0.0f),
	a(0.0f)
{
	*this = other;
}

ColorF::ColorF(const Math::Vector4& vector4)
	: r(0.0f),
	g(0.0f),
	b(0.0f),
	a(0.0f)
{
	*this = vector4;
}



ColorF& ColorF::Add(const ColorF& other)
{
	r = fabs(r + other.r);
	g = fabs(g + other.g);
	b = fabs(b + other.b);
	a = fabs(a + other.a);

	return *this;
}

ColorF& ColorF::Add(float scalar)
{
	r = fabs(r + scalar);
	g = fabs(g + scalar);
	b = fabs(b + scalar);
	a = fabs(a + scalar);

	return *this;
}

ColorF& ColorF::Subtract(const ColorF& other)
{
	r = fabs(r - other.r);
	g = fabs(g - other.g);
	b = fabs(b - other.b);
	a = fabs(a - other.a);

	return *this;
}

ColorF& ColorF::Subtract(float scalar)
{
	r = fabs(r - scalar);
	g = fabs(g - scalar);
	b = fabs(b - scalar);
	a = fabs(a - scalar);

	return *this;
}

ColorF& ColorF::Multiply(const ColorF& other)
{
	r = fabs(r * other.r);
	g = fabs(g * other.g);
	b = fabs(b * other.b);
	a = fabs(a * other.a);

	return *this;
}

ColorF& ColorF::Multiply(float scalar)
{
	r = fabs(r * scalar);
	g = fabs(g * scalar);
	b = fabs(b * scalar);
	a = fabs(a * scalar);

	return *this;
}

ColorF& ColorF::Divide(const ColorF& other)
{
	r = fabs(r / other.r);
	g = fabs(g / other.g);
	b = fabs(b / other.b);
	a = fabs(a / other.a);

	return *this;
}

ColorF& ColorF::Divide(float scalar)
{
	r = fabs(r / scalar);
	g = fabs(g / scalar);
	b = fabs(b / scalar);
	a = fabs(a / scalar);

	return *this;
}



ColorF& ColorF::Lerp(const ColorF& src, float delta)
{
	r = fabs(r + (src.r - r) * delta);
	g = fabs(g + (src.g - g) * delta);
	b = fabs(b + (src.b - b) * delta);
	a = fabs(a + (src.a - a) * delta);

	return *this;
}

ColorF ColorF::Lerp(const ColorF& src, float delta) const
{
	return ColorF(*this).Lerp(src, delta);
}



bool ColorF::Equals(const ColorF& other) const
{
	return (r == other.r && g == other.g && b == other.b && a == other.a);
}

std::string ColorF::ToString() const
{
	using namespace std;
	return string("[ColorF][" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ", " + to_string(a) + ']');
}



bool ColorF::operator==(const ColorF& other) const
{
	return Equals(other);
}

bool ColorF::operator!=(const ColorF& other) const
{
	return !Equals(other);
}



ColorF& ColorF::operator=(const Math::Vector4& other)
{
	r = fabs(other.x);
	g = fabs(other.y);
	b = fabs(other.z);
	a = fabs(other.w);

	return *this;
}



ColorF& ColorF::operator=(const ColorF& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;

	return *this;
}

ColorF& ColorF::operator+=(const ColorF& other)
{
	return Add(other);
}

ColorF& ColorF::operator-=(const ColorF& other)
{
	return Subtract(other);
}

ColorF& ColorF::operator*=(const ColorF& other)
{
	return Multiply(other);
}

ColorF& ColorF::operator/=(const ColorF& other)
{
	return Divide(other);
}

ColorF& ColorF::operator+=(float scalar)
{
	return Add(scalar);
}

ColorF& ColorF::operator-=(float scalar)
{
	return Subtract(scalar);
}

ColorF& ColorF::operator*=(float scalar)
{
	return Multiply(scalar);
}

ColorF& ColorF::operator/=(float scalar)
{
	return Divide(scalar);
}



ColorF::operator const float*() const
{
	return rgba;
}

ColorF::operator float*()
{
	Abs();

	return rgba;
}



ColorF::operator Math::Vector4&()
{
	Abs();

	return vector;
}

ColorF::operator const Math::Vector4&() const
{
	return vector;
}



ColorF operator-(ColorF left, const ColorF& right)
{
	return left.Subtract(right);
}
ColorF operator+(ColorF left, const ColorF& right)
{
	return left.Add(right);
}

ColorF operator*(ColorF left, const ColorF& right)
{
	return left.Multiply(right);
}

ColorF operator/(ColorF left, const ColorF& right)
{
	return left.Divide(right);
}

ColorF operator-(ColorF left, float right)
{
	return left.Subtract(right);
}

ColorF operator+(ColorF left, float right)
{
	return left.Add(right);
}

ColorF operator*(ColorF left, float right)
{
	return left.Multiply(right);
}

ColorF operator/(ColorF left, float right)
{
	return left.Divide(right);
}

ColorF operator*(float left, ColorF right)
{
	return right.Multiply(left);
}



void ColorF::Abs()
{
	r = fabs(r);
	g = fabs(g);
	b = fabs(b);
	a = fabs(a);
}