#include "Matrix2.h"

namespace Math
{
	Matrix2::Matrix2()
	{
		memset(m, 0, sizeof(float) * 4);
	}

	Matrix2::Matrix2(float diagonal)
	{
		memset(m, 0, sizeof(float) * 4);

		rows[0].v[0] = diagonal;
		rows[1].v[1] = diagonal;
	}

	Matrix2::Matrix2(const Vector2& r1, const Vector2& r2)
	{
		rows[0] = r1;
		rows[1] = r2;
	}

	Matrix2::Matrix2(const Matrix2& other)
	{
		*this = other;
	}



	Vector2 Matrix2::Multiply(const Vector2& vector) const
	{
		float t[2];

		t[0] = (vector.v[0] * rows[0].v[0]) + (vector.v[1] * rows[1].v[0]);

		t[1] = (vector.v[0] * rows[0].v[1]) + (vector.v[1] * rows[1].v[1]);

		return Vector2(t[0], t[1]);
	}

	Matrix2& Matrix2::Multiply(const Matrix2& other)
	{
		float t[4];

		t[0] = (rows[0].v[0] * other.rows[0].v[0]) + (rows[0].v[1] * other.rows[1].v[0]);
		t[1] = (rows[0].v[0] * other.rows[0].v[1]) + (rows[0].v[1] * other.rows[1].v[1]);

		t[2] = (rows[1].v[0] * other.rows[0].v[0]) + (rows[1].v[1] * other.rows[1].v[0]);
		t[3] = (rows[1].v[0] * other.rows[0].v[1]) + (rows[1].v[1] * other.rows[1].v[1]);

		memcpy(m, t, sizeof(float) * 4);

		return *this;
	}

	Matrix2& Matrix2::Multiply(float scalar)
	{
		for (char i = 0; i < 4; i++)
			m[i] *= scalar;

		return *this;
	}



	Matrix2& Matrix2::Add(const Matrix2& other)
	{
		for (char i = 0; i < 4; i++)
			m[i] += other.m[i];

		return *this;
	}

	Matrix2& Matrix2::Add(float scalar)
	{
		for (char i = 0; i < 4; i++)
			m[i] += scalar;

		return *this;
	}



	Matrix2& Matrix2::Subtract(const Matrix2& other)
	{
		for (char i = 0; i < 4; i++)
			m[i] -= other.m[i];

		return *this;
	}

	Matrix2& Matrix2::Subtract(float scalar)
	{
		for (char i = 0; i < 4; i++)
			m[i] -= scalar;

		return *this;
	}



	Matrix2& Matrix2::Divide(float scalar)
	{
		for (char i = 0; i < 4; i++)
			m[i] /= scalar;

		return *this;
	}



	bool Matrix2::Equals(const Matrix2& other) const
	{
		for (char i = 0; i < 4; i++)
		{
			if (m[i] != other.m[i])
				return false;
		}

		return true;
	}



	Matrix2& Matrix2::Transpose()
	{
		float temp[4];
	
		temp[0] = rows[0].v[0];
		temp[1] = rows[1].v[0];

		temp[2] = rows[0].v[1];
		temp[3] = rows[1].v[1];

		memcpy(m, temp, sizeof(float) * 4);
	
		return *this;
	}

	Matrix2 Matrix2::TransposeOf() const
	{
		return Matrix2(*this).Transpose();
	}



	Matrix2& Matrix2::Invert()
	{
		float det = Determinant();
	
		if (det == 0)
			*this = Matrix2::Nan();
		else
			*this = Adjugate().Divide(det);

		return *this;
	}

	Matrix2 Matrix2::InverseOf() const
	{
		return Matrix2(*this).Invert();
	}



	Matrix2 Matrix2::Cofactor() const
	{
		return Adjugate().Transpose();
	}

	Matrix2 Matrix2::Adjugate() const
	{
		Matrix2 a;

		a.rows[0].v[0] = rows[1].v[1];
		a.rows[1].v[0] = -rows[1].v[0];
		a.rows[0].v[1] = -rows[0].v[1];
		a.rows[1].v[1] = rows[0].v[0];

		return a;
	}



	float Matrix2::Determinant() const
	{
		return (rows[0].v[0] * rows[1].v[1]) - (rows[0].v[1] * rows[1].v[0]);
	}



	float* Matrix2::GetArray()
	{
		return m;
	}

	const float* Matrix2::GetArray() const
	{
		return m;
	}



	float& Matrix2::GetElement(unsigned char r, unsigned char c)
	{
		assert(r < 2);
		assert(c < 2);
	
		return rows[r].v[c];
	}

	float Matrix2::GetElement(unsigned char r, unsigned char c) const
	{
		assert(r < 2);
		assert(c < 2);

		return rows[r].v[c];
	}

	Vector2& Matrix2::GetRow(unsigned char r)
	{
		assert(r < 2);

		return rows[r];
	}

	Vector2 Matrix2::GetRow(unsigned char r) const
	{
		assert(r < 2);

		return rows[r];
	}



	std::string Matrix2::ToString() const
	{
		return std::string(rows[0].ToString() + '\n' + rows[1].ToString());
	}



	bool Matrix2::operator==(const Matrix2& other) const
	{
		return Equals(other);
	}

	bool Matrix2::operator!=(const Matrix2& other) const
	{
		return !Equals(other);
	}



	float Matrix2::operator()(unsigned char r, unsigned char c) const
	{
		return GetElement(r, c);
	}

	float& Matrix2::operator()(unsigned char r, unsigned char c)
	{
		return GetElement(r, c);
	}

	Vector2 Matrix2::operator()(unsigned char r) const
	{
		return GetRow(r);
	}

	Vector2& Matrix2::operator()(unsigned char r)
	{
		return GetRow(r);
	}



	Vector2 operator*(Vector2 left, const Matrix2& right)
	{
		return right.Multiply(left);
	}

	Vector2& operator*=(Vector2& left, const Matrix2& right)
	{
		return left = right.Multiply(left);
	}



	Matrix2 operator+(Matrix2 left, const Matrix2& right)
	{
		return left.Add(right);
	}

	Matrix2 operator-(Matrix2 left, const Matrix2& right)
	{
		return left.Subtract(right);
	}

	Matrix2 operator*(Matrix2 left, const Matrix2& right)
	{
		return left.Multiply(right);
	}



	Matrix2 operator+(Matrix2 left, float right)
	{
		return left.Add(right);
	}

	Matrix2 operator-(Matrix2 left, float right)
	{
		return left.Subtract(right);
	}

	Matrix2 operator*(Matrix2 left, float right)
	{
		return left.Multiply(right);
	}

	Matrix2 operator/(Matrix2 left, float right)
	{
		return left.Divide(right);
	}

	Matrix2 operator*(float left, Matrix2 right)
	{
		return right.Multiply(left);
	}

	Matrix2 operator/(float left, Matrix2 right)
	{
		return right.Divide(left);
	}



	Matrix2& Matrix2::operator=(const Matrix2& other)
	{
		if (this != &other)
			memcpy(m, other.m, sizeof(float) * 4);
	
		return *this;
	}

	Matrix2& Matrix2::operator+=(const Matrix2& right)
	{
		return Add(right);
	}

	Matrix2& Matrix2::operator-=(const Matrix2& right)
	{
		return Subtract(right);
	}

	Matrix2& Matrix2::operator*=(const Matrix2& right)
	{
		return Multiply(right);
	}



	Matrix2& Matrix2::operator+=(float right)
	{
		return Add(right);
	}

	Matrix2& Matrix2::operator-=(float right)
	{
		return Subtract(right);
	}

	Matrix2& Matrix2::operator*=(float right)
	{
		return Multiply(right);
	}

	Matrix2& Matrix2::operator/=(float right)
	{
		return Divide(right);
	}



	Matrix2 Matrix2::Nan()
	{
		return Matrix2(Vector2::Nan(), Vector2::Nan());
	}

	Matrix2 Matrix2::Identity()
	{
		return Matrix2(1.0f);
	}



	Matrix2 Matrix2::Rotation(float angleRad)
	{
		Matrix2 rot(1.0f);
	
		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		rot.m[0] = cosine;
		rot.m[1] = -sine;
		rot.m[2] = sine;
		rot.m[3] = cosine;
	
		return rot;
	}

	Matrix2 Matrix2::Scale(const Vector2& scale)
	{
		Matrix2 s(1.0f);
	
		s.m[0] = scale.x;
		s.m[3] = scale.y;
	
		return s;
	}

	Matrix2 Matrix2::Scale(float scale)
	{
		return Matrix2(scale);
	}
}