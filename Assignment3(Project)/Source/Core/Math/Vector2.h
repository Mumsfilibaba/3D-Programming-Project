#pragma once

#include <string>
#include <assert.h>

namespace Math
{
	class Vector2
	{
	public:
		Vector2(float x = 0.0f, float y = 0.0f);
		Vector2(const Vector2& other);
		~Vector2() {}

		Vector2& Add(const Vector2& other);
		Vector2& Add(float scalar);
		Vector2& Subtract(const Vector2& other);
		Vector2& Subtract(float scalar);
		Vector2& Multiply(float scalar);
		Vector2& Divide(float scalar);

		bool IsUnitVector() const;
		bool Equals(const Vector2& other) const;

		float Dot(const Vector2& other) const;
		float LengthSqrd() const;
		float Length() const;
		Vector2& Normalize();
		Vector2 UnitVector() const;
		Vector2 Normal() const;
		Vector2 Project(const Vector2& other) const;

		std::string ToString() const;

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		Vector2 operator-() const;
		Vector2& operator=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(float scalar);
		Vector2& operator+=(float scalar);
		Vector2& operator*=(float scalar);
		Vector2& operator/=(float scalar);

		friend Vector2 operator-(Vector2 left, const Vector2& right);
		friend Vector2 operator+(Vector2 left, const Vector2& right);
		friend Vector2 operator-(Vector2 left, float right);
		friend Vector2 operator+(Vector2 left, float right);
		friend Vector2 operator*(Vector2 left, float right);
		friend Vector2 operator/(Vector2 left, float right);
		friend Vector2 operator*(float left, Vector2 right);

	public:
		static Vector2 Nan();

	public:
		union
		{
			struct
			{
				float x;
				float y;
			};

			float v[2];
		};
	};
}