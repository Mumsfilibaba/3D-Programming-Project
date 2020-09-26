#include "Vector2.h"

namespace Math
{
	Vector2::Vector2(float x, float y)
		: x(x),
		y(y)
	{
	}

	Vector2::Vector2(const Vector2& other)
		: x(0.0f),
		y(0.0f)
	{
		*this = other;
	}



	Vector2& Vector2::Add(const Vector2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2& Vector2::Add(float scalar)
	{
		x += scalar;
		y += scalar;

		return *this;
	}

	Vector2& Vector2::Subtract(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2& Vector2::Subtract(float scalar)
	{
		x -= scalar;
		y -= scalar;

		return *this;
	}

	Vector2& Vector2::Multiply(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector2& Vector2::Divide(float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}



	bool Vector2::IsUnitVector() const
	{
		float length = Length();
		//Solves minimal rounding errors (Common floats has 6 decimals)
		return (length > 0.999999f && length < 1.000001);
	}

	bool Vector2::Equals(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}



	float Vector2::Dot(const Vector2& other) const
	{
		return (x * other.x) + (y * other.y);
	}



	float Vector2::LengthSqrd() const
	{
		return (x * x) + (y * y);
	}

	float Vector2::Length() const
	{
		return sqrt(LengthSqrd());
	}



	Vector2& Vector2::Normalize()
	{
		float length = Length();

		if (length > 0.0f)
		{
			x /= length;
			y /= length;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
		}

		return *this;
	}

	Vector2 Vector2::UnitVector() const
	{
		return Vector2(*this).Normalize();
	}



	Vector2 Vector2::Normal() const
	{
		return Vector2(-y, x);
	}

	Vector2 Vector2::Project(const Vector2& other) const
	{
		Vector2 n(other);
		n.Normalize();

		return Dot(n) * n;
	}



	std::string Vector2::ToString() const
	{
		using namespace std;
		return string('[' + to_string(x) + ", " + to_string(y) + ']');
	}



	bool Vector2::operator==(const Vector2& other) const
	{
		return Equals(other);
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return !Equals(other);
	}



	Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}

	Vector2& Vector2::operator=(const Vector2& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}

		return *this;
	}



	Vector2 operator-(Vector2 left, const Vector2& right)
	{
		return left.Subtract(right);
	}

	Vector2 operator+(Vector2 left, const Vector2& right)
	{
		return left.Add(right);
	}

	Vector2 operator-(Vector2 left, float right)
	{
		return left.Subtract(right);
	}

	Vector2 operator+(Vector2 left, float right)
	{
		return left.Add(right);
	}

	Vector2 operator*(Vector2 left, float right)
	{
		return left.Multiply(right);
	}

	Vector2 operator/(Vector2 left, float right)
	{
		return left.Divide(right);
	}

	Vector2 operator*(float left, Vector2 right)
	{
		return right.Multiply(left);
	}



	Vector2& Vector2::operator-=(const Vector2& other)
	{
		return Subtract(other);
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		return Add(other);
	}

	Vector2& Vector2::operator-=(float scalar)
	{
		return Subtract(scalar);
	}

	Vector2& Vector2::operator+=(float scalar)
	{
		return Add(scalar);;
	}

	Vector2& Vector2::operator*=(float scalar)
	{
		return Multiply(scalar);;
	}

	Vector2& Vector2::operator/=(float scalar)
	{
		return Divide(scalar);
	}



	Vector2 Vector2::Nan()
	{
		return Vector2(NAN, NAN);
	}
}