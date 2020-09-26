#pragma once

#include "Vector2.h"

namespace Math
{
	class Matrix2
	{
	public:
		Matrix2();
		Matrix2(float value);
		Matrix2(const Vector2& r1, const Vector2& r2);
		Matrix2(const Matrix2& other);
		~Matrix2() {}

		Vector2 Multiply(const Vector2& vector) const;

		Matrix2& Multiply(const Matrix2& other);
		Matrix2& Multiply(float scalar);
		Matrix2& Add(const Matrix2& other);
		Matrix2& Add(float scalar);
		Matrix2& Subtract(const Matrix2& other);
		Matrix2& Subtract(float scalar);
		Matrix2& Divide(float scalar);

		bool Equals(const Matrix2& other) const;

		Matrix2& Transpose();
		Matrix2& Invert();
		Matrix2 TransposeOf() const;
		Matrix2 InverseOf() const;

		Matrix2 Cofactor() const;
		Matrix2 Adjugate() const;

		float Determinant() const;

		float* GetArray();
		const float* GetArray() const;
		float& GetElement(unsigned char r, unsigned char c);
		float GetElement(unsigned char r, unsigned char c) const;
		Vector2& GetRow(unsigned char r);
		Vector2 GetRow(unsigned char r) const;

		std::string ToString() const;
	
		bool operator==(const Matrix2& other) const;
		bool operator!=(const Matrix2& other) const;

		float& operator()(unsigned char r, unsigned char c);
		float operator()(unsigned char r, unsigned char c) const;
		Vector2& operator()(unsigned char r);
		Vector2 operator()(unsigned char r) const;

		Matrix2& operator=(const Matrix2& other);
		Matrix2& operator+=(const Matrix2& right);
		Matrix2& operator-=(const Matrix2& right);
		Matrix2& operator*=(const Matrix2& right);
		Matrix2& operator+=(float right);
		Matrix2& operator-=(float right);
		Matrix2& operator*=(float right);
		Matrix2& operator/=(float right);


		friend Vector2& operator*=(Vector2& left, const Matrix2& right);
		friend Vector2 operator*(Vector2 left, const Matrix2& right);

		friend Matrix2 operator+(Matrix2 left, const Matrix2& right);
		friend Matrix2 operator-(Matrix2 left, const Matrix2& right);
		friend Matrix2 operator*(Matrix2 left, const Matrix2& right);
		friend Matrix2 operator+(Matrix2 left, float right);
		friend Matrix2 operator-(Matrix2 left, float right);
		friend Matrix2 operator*(Matrix2 left, float right);
		friend Matrix2 operator/(Matrix2 left, float right);
		friend Matrix2 operator*(float left, Matrix2 right);
		friend Matrix2 operator/(float left, Matrix2 right);

	public:
		static Matrix2 Nan();
		static Matrix2 Scale(const Vector2& scale);
		static Matrix2 Scale(float scale);
		static Matrix2 Identity();
		static Matrix2 Rotation(float angleRad);

	private:
		union
		{
			float m[4];
			Vector2 rows[2];
		};
	};
}