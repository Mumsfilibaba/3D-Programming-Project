#include "Color.h"

const Color Color::BLACK(0, 0, 0, 255);
const Color Color::WHITE(255, 255, 255, 255);
const Color Color::GRAY(128, 128, 128, 255);
const Color Color::DARKGRAY(64, 64, 64, 255);
const Color Color::SOFTGRAY(192, 192, 192, 255);
const Color Color::RED(255, 0, 0, 255);
const Color Color::DARKRED(128, 0, 0, 255);
const Color Color::SOFTRED(255, 64, 64, 255);
const Color Color::GREEN(0, 255, 0, 255);
const Color Color::SOFTGREEN(64, 255, 64, 255);
const Color Color::DARKGREEN(0, 128, 0, 255);
const Color Color::BLUE(0, 0, 255, 255);
const Color Color::DARKBLUE(0, 0, 128, 255);
const Color Color::SOFTBLUE(83, 83, 255, 255);
const Color Color::CORNFLOWERBLUE(100, 149, 237, 255);
const Color Color::YELLOW(255, 255, 0, 255);
const Color Color::SOFTYELLOW(255, 255, 64, 255);
const Color Color::DARKYELLOW(128, 128, 0, 255);
const Color Color::MAGENTA(255, 0, 255, 255);
const Color Color::PURPLE(128, 0, 128, 255);
const Color Color::PINK(255, 192, 203, 255);



Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
	: r(r),
	g(g),
	b(b),
	a(a)
{
}

Color::Color(const Color& other)
	: color(0)
{
	*this = other;
}

Color::Color(const ColorF& colorf)
	: color(0)
{
	*this = colorf;
}

Color::Color(const Math::Vector4& vector4)
	: color(0)
{
	*this = vector4;
}



Color& Color::Add(const Color& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;

	return *this;
}

Color& Color::Add(uint8 scalar)
{
	r += scalar;
	g += scalar;
	b += scalar;
	a += scalar;

	return *this;
}

Color& Color::Subtract(const Color& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;

	return *this;
}

Color& Color::Subtract(uint8 scalar)
{
	r -= scalar;
	g -= scalar;
	b -= scalar;
	a -= scalar;

	return *this;
}

Color& Color::Multiply(const Color& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;

	return *this;
}

Color& Color::Multiply(uint8 scalar)
{
	r *= scalar;
	g *= scalar;
	b *= scalar;
	a *= scalar;

	return *this;
}

Color& Color::Divide(const Color& other)
{
	r /= other.r;
	g /= other.g;
	b /= other.b;
	a /= other.a;

	return *this;
}

Color& Color::Divide(uint8 scalar)
{
	r /= scalar;
	g /= scalar;
	b /= scalar;
	a /= scalar;

	return *this;
}



bool Color::Equals(const Color& other) const
{
	return (color == other.color);
}

std::string Color::ToString() const
{
	using namespace std;
	return string("[Color][" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ", " + to_string(a) + ']');
}



bool Color::operator==(const Color& other) const
{
	return Equals(other);
}

bool Color::operator!=(const Color& other) const
{
	return !Equals(other);
}



Color& Color::operator=(const Math::Vector4& other)
{
	r = (uint8)(other.x * 255);
	g = (uint8)(other.y * 255);
	b = (uint8)(other.z * 255);
	a = (uint8)(other.w * 255);

	return *this;
}

Color& Color::operator=(const ColorF& other)
{
	r = (uint8)(other.r * 255);
	g = (uint8)(other.g * 255);
	b = (uint8)(other.b * 255);
	a = (uint8)(other.a * 255);

	return *this;
}



Color& Color::operator=(const Color& other)
{
	if (this != &other)
		color = other.color;

	return *this;
}

Color& Color::operator+=(const Color& other)
{
	return Add(other);
}

Color& Color::operator-=(const Color& other)
{
	return Subtract(other);
}

Color& Color::operator*=(const Color& other)
{
	return Multiply(other);
}

Color& Color::operator/=(const Color& other)
{
	return Divide(other);
}

Color& Color::operator+=(uint8 scalar)
{
	return Add(scalar);
}

Color& Color::operator-=(uint8 scalar)
{
	return Subtract(scalar);
}

Color& Color::operator*=(uint8 scalar)
{
	return Multiply(scalar);
}

Color& Color::operator/=(uint8 scalar)
{
	return Divide(scalar);
}



Color::operator ColorF()
{
	ColorF result;

	result.r = r / 255.0f;
	result.g = g / 255.0f;
	result.b = b / 255.0f;
	result.a = a / 255.0f;

	return result;
}



Color operator-(Color left, const Color& right)
{
	return left.Subtract(right);
}
Color operator+(Color left, const Color& right)
{
	return left.Add(right);
}

Color operator*(Color left, const Color& right)
{
	return left.Multiply(right);
}

Color operator/(Color left, const Color& right)
{
	return left.Divide(right);
}

Color operator-(Color left, uint8 right)
{
	return left.Subtract(right);
}

Color operator+(Color left, uint8 right)
{
	return left.Add(right);
}

Color operator*(Color left, uint8 right)
{
	return left.Multiply(right);
}

Color operator/(Color left, uint8 right)
{
	return left.Divide(right);
}

Color operator*(uint8 left, Color right)
{
	return right.Multiply(left);
}