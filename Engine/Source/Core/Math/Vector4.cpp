#include "Vector4.h"

namespace Math
{
	Vector4::Vector4(float x, float y, float z, float w)
		: x(x),
		y(y),
		z(z),
		w(w)
	{
	}

	Vector4::Vector4(const Vector4& other)
		: x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
		*this = other;
	}

	Vector4::Vector4(const Vector3& vector3)
		: x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
		*this = vector3;
	}



	Vector4& Vector4::Add(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vector4& Vector4::Add(float scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		w += scalar;

		return *this;
	}

	Vector4& Vector4::Subtract(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vector4& Vector4::Subtract(float scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		w -= scalar;

		return *this;
	}

	Vector4& Vector4::Multiply(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Vector4& Vector4::Divide(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}



	bool Vector4::IsUnitVector() const
	{
		float length = Length();
		//To fix rounding errors
		return length > 0.999999f && length < 1.000001;
	}

	bool Vector4::Equals(const Vector4& other) const
	{
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}



	float Vector4::Dot(const Vector4& other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
	}



	float Vector4::LengthSqrd() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	float Vector4::Length() const
	{
		return sqrt(LengthSqrd());
	}



	Vector4& Vector4::Normalize()
	{
		float length = Length();

		if (length > 0)
		{
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		return *this;
	}

	Vector4 Vector4::UnitVector() const
	{
		return Vector4(*this).Normalize();
	}



	Vector4 Vector4::Project(const Vector4& other) const
	{
		Vector4 n(other);
		n.Normalize();

		return Dot(n) * n;
	}



	std::string Vector4::ToString() const
	{
		using namespace std;
		return string('[' + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ", " + to_string(w) + ']');
	}



	bool Vector4::operator==(const Vector4& other) const
	{
		return Equals(other);
	}

	bool Vector4::operator!=(const Vector4& other) const
	{
		return !Equals(other);
	}



	Vector4 operator-(Vector4 left, const Vector4& right)
	{
		return left.Subtract(right);
	}

	Vector4 operator+(Vector4 left, const Vector4& right)
	{
		return left.Add(right);
	}

	Vector4 operator-(Vector4 left, float right)
	{
		return left.Subtract(right);
	}

	Vector4 operator+(Vector4 left, float right)
	{
		return left.Add(right);
	}

	Vector4 operator*(Vector4 left, float right)
	{
		return left.Multiply(right);
	}

	Vector4 operator/(Vector4 left, float right)
	{
		return left.Divide(right);
	}

	Vector4 operator*(float left, Vector4 right)
	{
		return right.Multiply(left);
	}



	Vector4& Vector4::operator=(const Vector3& vector3)
	{
		xyz = vector3;
		w = 1.0f;

		return *this;
	}

	Vector4& Vector4::operator=(const Vector4& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& other)
	{
		return Subtract(other);
	}

	Vector4& Vector4::operator+=(const Vector4& other)
	{
		return Add(other);
	}

	Vector4& Vector4::operator-=(float scalar)
	{
		return Subtract(scalar);
	}

	Vector4& Vector4::operator+=(float scalar)
	{
		return Add(scalar);
	}

	Vector4& Vector4::operator*=(float scalar)
	{
		return Multiply(scalar);
	}

	Vector4& Vector4::operator/=(float scalar)
	{
		return Divide(scalar);
	}



	Vector4::operator Vector3&()
	{
		return xyz;
	}

	Vector4::operator const Vector3&() const
	{
		return xyz;
	}



	Vector4 Vector4::Nan()
	{
		return Vector4(NAN, NAN, NAN, NAN);
	}
}