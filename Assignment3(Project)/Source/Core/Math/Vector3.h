#pragma once 

#include <string>
#include <assert.h>

namespace Math
{
	class Vector3
	{
	public:
		Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Vector3(const Vector3& other);
		~Vector3() {}

		Vector3& Add(const Vector3& other);
		Vector3& Add(float scalar);
		Vector3& Subtract(const Vector3& other);
		Vector3& Subtract(float scalar);
		Vector3& Multiply(float scalar);
		Vector3& Divide(float scalar);

		bool IsUnitVector() const;
		bool Equals(const Vector3& other) const;

		float Dot(const Vector3& other) const;
		float LengthSqrd() const;
		float Length() const;
		Vector3& Normalize();
		Vector3 UnitVector() const;
		Vector3 Cross(const Vector3& other) const;
		Vector3 Project(const Vector3& other) const;

		std::string ToString() const;

		Vector3 operator-() const;
		Vector3& operator=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(float other);
		Vector3& operator+=(float other);
		Vector3& operator*=(float other);
		Vector3& operator/=(float other);

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;


		friend Vector3 operator-(Vector3 left, const Vector3& right);
		friend Vector3 operator+(Vector3 left, const Vector3& right);
		friend Vector3 operator-(Vector3 left, float right);
		friend Vector3 operator+(Vector3 left, float right);
		friend Vector3 operator*(Vector3 left, float right);
		friend Vector3 operator/(Vector3 left, float right);
		friend Vector3 operator*(float left, Vector3 right);

	public:
		static Vector3 Nan();

	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			float v[3];
		};

	public:
		static const Vector3 UP;
		static const Vector3 FORWARD;
		static const Vector3 RIGHT;
	};
}