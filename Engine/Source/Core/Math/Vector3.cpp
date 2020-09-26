#include "Vector3.h"

namespace Math
{
	const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);



	Vector3::Vector3(float x, float y, float z)
		: x(x),
		y(y),
		z(z)
	{
	}

	Vector3::Vector3(const Vector3& other)
		: x(0.0f),
		y(0.0f),
		z(0.0f)
	{
		*this = other;
	}



	Vector3& Vector3::Add(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector3& Vector3::Add(float scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;

		return *this;
	}

	Vector3& Vector3::Subtract(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector3& Vector3::Subtract(float scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;

		return *this;
	}

	Vector3& Vector3::Multiply(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector3& Vector3::Divide(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}



	bool Vector3::IsUnitVector() const
	{
		float length = Length();
		//Solves minimal rounding errors (Common floats has 6 decimals)
		return (length > 0.999999f && length < 1.000001);
	}

	bool Vector3::Equals(const Vector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}



	float Vector3::Dot(const Vector3& other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}



	float Vector3::LengthSqrd() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	float Vector3::Length() const
	{
		return sqrt(LengthSqrd());
	}



	Vector3& Vector3::Normalize()
	{
		float length = Length();
		if (length > 0.0f)
		{
			x /= length;
			y /= length;
			z /= length;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		return *this;
	}

	Vector3 Vector3::UnitVector() const
	{
		return Vector3(*this).Normalize();
	}



	Vector3 Vector3::Cross(const Vector3& other) const
	{
		return Vector3(
			(y * other.z) - (other.y * z),
			(z * other.x) - (other.z * x),
			(x * other.y) - (other.x * y));
	}

	Vector3 Vector3::Project(const Vector3& other) const
	{
		Vector3 n(other);
		n.Normalize();

		return Dot(n) * n;
	}



	std::string Vector3::ToString() const
	{
		using namespace std;
		return string('[' + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ']');
	}



	Vector3 operator-(Vector3 left, const Vector3& right)
	{
		return left.Subtract(right);
	}

	Vector3 operator+(Vector3 left, const Vector3& right)
	{
		return left.Add(right);
	}

	Vector3 operator-(Vector3 left, float right)
	{
		return left.Subtract(right);
	}

	Vector3 operator+(Vector3 left, float right)
	{
		return left.Add(right);
	}

	Vector3 operator*(Vector3 left, float right)
	{
		return left.Multiply(right);
	}

	Vector3 operator/(Vector3 left, float right)
	{
		return left.Divide(right);
	}

	Vector3 operator*(float left, Vector3 right)
	{
		return right.Multiply(left);
	}



	Vector3& Vector3::operator=(const Vector3& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		return *this;
	}



	Vector3& Vector3::operator-=(const Vector3& other)
	{
		return Subtract(other);
	}

	Vector3& Vector3::operator+=(const Vector3& other)
	{
		return Add(other);
	}

	Vector3& Vector3::operator-=(float scalar)
	{
		return Subtract(scalar);
	}

	Vector3& Vector3::operator+=(float scalar)
	{
		return Add(scalar);
	}

	Vector3& Vector3::operator*=(float scalar)
	{
		return Multiply(scalar);
	}

	Vector3& Vector3::operator/=(float scalar)
	{
		return Divide(scalar);
	}



	bool Vector3::operator==(const Vector3& other) const
	{
		return Equals(other);
	}

	bool Vector3::operator!=(const Vector3& other) const
	{
		return !Equals(other);
	}



	Vector3 Vector3::Nan()
	{
		return Vector3(NAN, NAN, NAN);
	}
}