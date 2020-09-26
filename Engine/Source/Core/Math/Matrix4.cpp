#include "MathUtilities.h"
#include "Matrix4.h"

namespace Math
{
	Matrix4::Matrix4()
	{
		ZeroBlock(sizeof(float) * 16, &m);
	}

	Matrix4::Matrix4(float diagonal)
	{
		ZeroBlock(sizeof(float) * 16, &m);

		rows[0].v[0] = diagonal;
		rows[1].v[1] = diagonal;
		rows[2].v[2] = diagonal;
		rows[3].v[3] = diagonal;
	}

	Matrix4::Matrix4(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4)
	{
		rows[0] = r1;
		rows[1] = r2;
		rows[2] = r3;
		rows[3] = r4;
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		*this = other;
	}



	Vector4 Matrix4::Multiply(const Vector4& vector) const
	{
		float t[4];

		t[0] = (rows[0].v[0] * vector.v[0]) + (rows[0].v[1] * vector.v[1]) + (rows[0].v[2] * vector.v[2]) + (rows[0].v[3] * vector.v[3]);

		t[1] = (rows[1].v[0] * vector.v[0]) + (rows[1].v[1] * vector.v[1]) + (rows[1].v[2] * vector.v[2]) + (rows[1].v[3] * vector.v[3]);

		t[2] = (rows[2].v[0] * vector.v[0]) + (rows[2].v[1] * vector.v[1]) + (rows[2].v[2] * vector.v[2]) + (rows[2].v[3] * vector.v[3]);

		t[3] = (rows[3].v[0] * vector.v[0]) + (rows[3].v[1] * vector.v[1]) + (rows[3].v[2] * vector.v[2]) + (rows[3].v[3] * vector.v[3]);

		return Vector4(t[0], t[1], t[2], t[3]);
	}



	Matrix4& Matrix4::Multiply(const Matrix4& other)
	{
		float t[16];

		t[0] = (rows[0].v[0] * other.rows[0].v[0]) + (rows[0].v[1] * other.rows[1].v[0]) + (rows[0].v[2] * other.rows[2].v[0]) + (rows[0].v[3] * other.rows[3].v[0]);
		t[1] = (rows[0].v[0] * other.rows[0].v[1]) + (rows[0].v[1] * other.rows[1].v[1]) + (rows[0].v[2] * other.rows[2].v[1]) + (rows[0].v[3] * other.rows[3].v[1]);
		t[2] = (rows[0].v[0] * other.rows[0].v[2]) + (rows[0].v[1] * other.rows[1].v[2]) + (rows[0].v[2] * other.rows[2].v[2]) + (rows[0].v[3] * other.rows[3].v[2]);
		t[3] = (rows[0].v[0] * other.rows[0].v[3]) + (rows[0].v[1] * other.rows[1].v[3]) + (rows[0].v[2] * other.rows[2].v[3]) + (rows[0].v[3] * other.rows[3].v[3]);

		t[4] = (rows[1].v[0] * other.rows[0].v[0]) + (rows[1].v[1] * other.rows[1].v[0]) + (rows[1].v[2] * other.rows[2].v[0]) + (rows[1].v[3] * other.rows[3].v[0]);
		t[5] = (rows[1].v[0] * other.rows[0].v[1]) + (rows[1].v[1] * other.rows[1].v[1]) + (rows[1].v[2] * other.rows[2].v[1]) + (rows[1].v[3] * other.rows[3].v[1]);
		t[6] = (rows[1].v[0] * other.rows[0].v[2]) + (rows[1].v[1] * other.rows[1].v[2]) + (rows[1].v[2] * other.rows[2].v[2]) + (rows[1].v[3] * other.rows[3].v[2]);
		t[7] = (rows[1].v[0] * other.rows[0].v[3]) + (rows[1].v[1] * other.rows[1].v[3]) + (rows[1].v[2] * other.rows[2].v[3]) + (rows[1].v[3] * other.rows[3].v[3]);

		t[8] = (rows[2].v[0] * other.rows[0].v[0]) + (rows[2].v[1] * other.rows[1].v[0]) + (rows[2].v[2] * other.rows[2].v[0]) + (rows[2].v[3] * other.rows[3].v[0]);
		t[9] = (rows[2].v[0] * other.rows[0].v[1]) + (rows[2].v[1] * other.rows[1].v[1]) + (rows[2].v[2] * other.rows[2].v[1]) + (rows[2].v[3] * other.rows[3].v[1]);
		t[10] = (rows[2].v[0] * other.rows[0].v[2]) + (rows[2].v[1] * other.rows[1].v[2]) + (rows[2].v[2] * other.rows[2].v[2]) + (rows[2].v[3] * other.rows[3].v[2]);
		t[11] = (rows[2].v[0] * other.rows[0].v[3]) + (rows[2].v[1] * other.rows[1].v[3]) + (rows[2].v[2] * other.rows[2].v[3]) + (rows[2].v[3] * other.rows[3].v[3]);

		t[12] = (rows[3].v[0] * other.rows[0].v[0]) + (rows[3].v[1] * other.rows[1].v[0]) + (rows[3].v[2] * other.rows[2].v[0]) + (rows[3].v[3] * other.rows[3].v[0]);
		t[13] = (rows[3].v[0] * other.rows[0].v[1]) + (rows[3].v[1] * other.rows[1].v[1]) + (rows[3].v[2] * other.rows[2].v[1]) + (rows[3].v[3] * other.rows[3].v[1]);
		t[14] = (rows[3].v[0] * other.rows[0].v[2]) + (rows[3].v[1] * other.rows[1].v[2]) + (rows[3].v[2] * other.rows[2].v[2]) + (rows[3].v[3] * other.rows[3].v[2]);
		t[15] = (rows[3].v[0] * other.rows[0].v[3]) + (rows[3].v[1] * other.rows[1].v[3]) + (rows[3].v[2] * other.rows[2].v[3]) + (rows[3].v[3] * other.rows[3].v[3]);

		memcpy(m, t, sizeof(float) * 16);

		return *this;
	}

	Matrix4& Matrix4::Multiply(float scalar)
	{
		for (char i = 0; i < 16; i++)
			m[i] *= scalar;

		return *this;
	}

	Matrix4& Matrix4::Add(const Matrix4& other)
	{
		for (char i = 0; i < 16; i++)
			m[i] += other.m[i];

		return *this;
	}

	Matrix4& Matrix4::Add(float scalar)
	{
		for (char i = 0; i < 16; i++)
			m[i] += scalar;

		return *this;
	}

	Matrix4& Matrix4::Subtract(const Matrix4& other)
	{
		for (char i = 0; i < 16; i++)
			m[i] -= other.m[i];

		return *this;
	}

	Matrix4& Matrix4::Subtract(float scalar)
	{
		for (char i = 0; i < 16; i++)
			m[i] -= scalar;

		return *this;
	}

	Matrix4& Matrix4::Divide(float scalar)
	{
		for (char i = 0; i < 16; i++)
			m[i] /= scalar;

		return *this;
	}



	bool Matrix4::Equals(const Matrix4& other) const
	{
		for (char i = 0; i < 16; i++)
		{
			if (m[i] != other.m[i])
				return false;
		}

		return true;
	}



	float* Matrix4::GetArray()
	{
		return m;
	}

	const float* Matrix4::GetArray() const
	{
		return m;
	}

	float Matrix4::GetElement(uint8 r, uint8 c) const
	{
		assert(r < 4);
		assert(c < 4);

		return rows[r].v[c];
	}

	float& Matrix4::GetElement(uint8 r, uint8 c)
	{
		assert(r < 4);
		assert(c < 4);

		return rows[r].v[c];
	}

	Vector4 Matrix4::GetRow(uint8 r) const
	{
		assert(r < 4);

		return rows[r];
	}

	Vector4& Matrix4::GetRow(uint8 r)
	{
		assert(r < 4);

		return rows[r];
	}



	Matrix4& Matrix4::Transpose()
	{
		float temp[16];

		temp[0] = rows[0].v[0];
		temp[1] = rows[1].v[0];
		temp[2] = rows[2].v[0];
		temp[3] = rows[3].v[0];

		temp[4] = rows[0].v[1];
		temp[5] = rows[1].v[1];
		temp[6] = rows[2].v[1];
		temp[7] = rows[3].v[1];

		temp[8] = rows[0].v[2];
		temp[9] = rows[1].v[2];
		temp[10] = rows[2].v[2];
		temp[11] = rows[3].v[2];

		temp[12] = rows[0].v[3];
		temp[13] = rows[1].v[3];
		temp[14] = rows[2].v[3];
		temp[15] = rows[3].v[3];

		memcpy(m, temp, sizeof(float) * 16);

		return *this;
	}

	Matrix4 Matrix4::TransposeOf() const
	{
		return Matrix4(*this).Transpose();
	}



	Matrix4& Matrix4::Invert()
	{
		float det = Determinant();

		if (det == 0.0f)
			*this = Matrix4::Nan();
		else
			*this = (Adjugate() / det);

		return *this;
	}

	Matrix4 Matrix4::InverseOf() const
	{
		return Matrix4(*this).Invert();
	}



	Matrix4 Matrix4::Cofactor() const
	{
		return Adjugate().Transpose();
	}

	Matrix4 Matrix4::Adjugate() const
	{
		Matrix4 adj;

		//d11
		adj.rows[0].v[0] = (rows[1].v[1] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2])));
		adj.rows[0].v[0] -= (rows[1].v[2] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1])));
		adj.rows[0].v[0] += (rows[1].v[3] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1])));
		//d12
		adj.rows[1].v[0] = (rows[1].v[0] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2])));
		adj.rows[1].v[0] -= (rows[1].v[2] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0])));
		adj.rows[1].v[0] += (rows[1].v[3] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0])));
		adj.rows[1].v[0] *= -1.0f;
		//d13
		adj.rows[2].v[0] = (rows[1].v[0] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1])));
		adj.rows[2].v[0] -= (rows[1].v[1] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0])));
		adj.rows[2].v[0] += (rows[1].v[3] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0])));
		//d14
		adj.rows[3].v[0] = (rows[1].v[0] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1])));
		adj.rows[3].v[0] -= (rows[1].v[1] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0])));
		adj.rows[3].v[0] += (rows[1].v[2] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0])));
		adj.rows[3].v[0] *= -1.0f;


		//d21
		adj.rows[0].v[1] = (rows[0].v[1] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2])));
		adj.rows[0].v[1] -= (rows[0].v[2] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1])));
		adj.rows[0].v[1] += (rows[0].v[3] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1])));
		adj.rows[0].v[1] *= -1.0f;
		//d22
		adj.rows[1].v[1] = (rows[0].v[0] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2])));
		adj.rows[1].v[1] -= (rows[0].v[2] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0])));
		adj.rows[1].v[1] += (rows[0].v[3] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0])));
		//d23
		adj.rows[2].v[1] = (rows[0].v[0] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1])));
		adj.rows[2].v[1] -= (rows[0].v[1] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0])));
		adj.rows[2].v[1] += (rows[0].v[3] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0])));
		adj.rows[2].v[1] *= -1.0f;
		//d24
		adj.rows[3].v[1] = (rows[0].v[0] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1])));
		adj.rows[3].v[1] -= (rows[0].v[1] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0])));
		adj.rows[3].v[1] += (rows[0].v[2] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0])));

		//d31
		adj.rows[0].v[2] = (rows[0].v[1] * ((rows[1].v[2] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[2])));
		adj.rows[0].v[2] -= (rows[0].v[2] * ((rows[1].v[1] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[1])));
		adj.rows[0].v[2] += (rows[0].v[3] * ((rows[1].v[1] * rows[3].v[2]) - (rows[1].v[2] * rows[3].v[1])));
		//d32
		adj.rows[1].v[2] = (rows[0].v[0] * ((rows[1].v[2] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[2])));
		adj.rows[1].v[2] -= (rows[0].v[2] * ((rows[1].v[0] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[0])));
		adj.rows[1].v[2] += (rows[0].v[3] * ((rows[1].v[0] * rows[3].v[2]) - (rows[1].v[2] * rows[3].v[0])));
		adj.rows[1].v[2] *= -1.0f;
		//d33
		adj.rows[2].v[2] = (rows[0].v[0] * ((rows[1].v[1] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[1])));
		adj.rows[2].v[2] -= (rows[0].v[1] * ((rows[1].v[0] * rows[3].v[3]) - (rows[1].v[3] * rows[3].v[0])));
		adj.rows[2].v[2] += (rows[0].v[3] * ((rows[1].v[0] * rows[3].v[1]) - (rows[1].v[1] * rows[3].v[0])));
		//d34
		adj.rows[3].v[2] = (rows[0].v[0] * ((rows[1].v[1] * rows[3].v[2]) - (rows[1].v[2] * rows[3].v[1])));
		adj.rows[3].v[2] -= (rows[0].v[1] * ((rows[1].v[0] * rows[3].v[2]) - (rows[1].v[2] * rows[3].v[0])));
		adj.rows[3].v[2] += (rows[0].v[2] * ((rows[1].v[0] * rows[3].v[1]) - (rows[1].v[1] * rows[3].v[0])));
		adj.rows[3].v[2] *= -1.0f;

		//d41
		adj.rows[0].v[3] = (rows[0].v[1] * ((rows[1].v[2] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[2])));
		adj.rows[0].v[3] -= (rows[0].v[2] * ((rows[1].v[1] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[1])));
		adj.rows[0].v[3] += (rows[0].v[3] * ((rows[1].v[1] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[1])));
		adj.rows[0].v[3] *= -1.0f;
		//d42
		adj.rows[1].v[3] = (rows[0].v[0] * ((rows[1].v[2] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[2])));
		adj.rows[1].v[3] -= (rows[0].v[2] * ((rows[1].v[0] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[0])));
		adj.rows[1].v[3] += (rows[0].v[3] * ((rows[1].v[0] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[0])));
		//d43
		adj.rows[2].v[3] = (rows[0].v[0] * ((rows[1].v[1] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[1])));
		adj.rows[2].v[3] -= (rows[0].v[1] * ((rows[1].v[0] * rows[2].v[3]) - (rows[1].v[3] * rows[2].v[0])));
		adj.rows[2].v[3] += (rows[0].v[3] * ((rows[1].v[0] * rows[2].v[1]) - (rows[1].v[1] * rows[2].v[0])));
		adj.rows[2].v[3] *= -1.0f;
		//d44
		adj.rows[3].v[3] = (rows[0].v[0] * ((rows[1].v[1] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[1])));
		adj.rows[3].v[3] -= (rows[0].v[1] * ((rows[1].v[0] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[0])));
		adj.rows[3].v[3] += (rows[0].v[2] * ((rows[1].v[0] * rows[2].v[1]) - (rows[1].v[1] * rows[2].v[0])));


		return adj;
	}



	float Matrix4::Determinant() const
	{
		float det = 0.0f;

		//d11
		det = rows[0].v[0] * (
			(rows[1].v[1] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2]))) -
			(rows[1].v[2] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1]))) +
			(rows[1].v[3] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1]))));
		//d12
		det -= rows[0].v[1] * (
			(rows[1].v[0] * ((rows[2].v[2] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[2]))) -
			(rows[1].v[2] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0]))) +
			(rows[1].v[3] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0]))));
		//d13
		det += rows[0].v[2] * (
			(rows[1].v[0] * ((rows[2].v[1] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[1]))) -
			(rows[1].v[1] * ((rows[2].v[0] * rows[3].v[3]) - (rows[2].v[3] * rows[3].v[0]))) +
			(rows[1].v[3] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0]))));
		//d14
		det -= rows[0].v[3] * (
			(rows[1].v[0] * ((rows[2].v[1] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[1]))) -
			(rows[1].v[1] * ((rows[2].v[0] * rows[3].v[2]) - (rows[2].v[2] * rows[3].v[0]))) +
			(rows[1].v[2] * ((rows[2].v[0] * rows[3].v[1]) - (rows[2].v[1] * rows[3].v[0]))));

		return det;
	}



	std::string Matrix4::ToString() const
	{
		return std::string(rows[0].ToString() + '\n' + rows[1].ToString() + '\n' + rows[2].ToString() + '\n' + rows[3].ToString());
	}



	bool Matrix4::operator==(const Matrix4& other) const
	{
		return Equals(other);
	}

	bool Matrix4::operator!=(const Matrix4& other) const
	{
		return !Equals(other);
	}



	float Matrix4::operator()(uint32 r, uint32 c) const
	{
		return GetElement(r, c);
	}

	float& Matrix4::operator()(uint32 r, uint32 c)
	{
		return GetElement(r, c);
	}

	Vector4 Matrix4::operator()(uint32 r) const
	{
		return GetRow(r);
	}

	Vector4& Matrix4::operator()(uint32 r)
	{
		return GetRow(r);
	}



	Vector4 operator*(const Vector4& left, const Matrix4& right)
	{
		return right.Multiply(left);
	}

	Vector4& operator*=(Vector4& left, const Matrix4& right)
	{
		return left = right.Multiply(left);
	}



	Matrix4 operator+(Matrix4 left, const Matrix4& right)
	{
		return left.Add(right);
	}

	Matrix4 operator-(Matrix4 left, const Matrix4& right)
	{
		return left.Subtract(right);
	}

	Matrix4 operator*(Matrix4 left, const Matrix4& right)
	{
		return left.Multiply(right);
	}



	Matrix4 operator+(Matrix4 left, float right)
	{
		return left.Add(right);
	}

	Matrix4 operator-(Matrix4 left, float right)
	{
		return left.Subtract(right);
	}

	Matrix4 operator*(Matrix4 left, float right)
	{
		return left.Multiply(right);
	}

	Matrix4 operator/(Matrix4 left, float right)
	{
		return left.Divide(right);
	}

	Matrix4 operator*(float left, Matrix4 right)
	{
		return right.Multiply(left);
	}



	Matrix4& Matrix4::operator=(const Matrix4& other)
	{
		if (this != &other)
			memcpy(m, other.m, sizeof(float) * 16);

		return *this;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& right)
	{
		return Add(right);
	}

	Matrix4& Matrix4::operator-=(const Matrix4& right)
	{
		return Subtract(right);
	}

	Matrix4& Matrix4::operator*=(const Matrix4& right)
	{
		return Multiply(right);
	}



	Matrix4& Matrix4::operator+=(float right)
	{
		return Add(right);
	}

	Matrix4& Matrix4::operator-=(float right)
	{
		return Subtract(right);
	}

	Matrix4& Matrix4::operator*=(float right)
	{
		return Multiply(right);
	}

	Matrix4& Matrix4::operator/=(float right)
	{
		return Divide(right);
	}



	Matrix4 Matrix4::Nan()
	{
		return Matrix4(Vector4::Nan(), Vector4::Nan(), Vector4::Nan(), Vector4::Nan());
	}

	Matrix4 Matrix4::Identity()
	{
		return Matrix4(1);
	}



	Matrix4 Matrix4::Perspective(float fovRad, float aspectWiHe, float farZ, float nearZ)
	{
		if (fovRad < (PI / 180.0) || fovRad >(PI - (PI / 180.0)))
			return Matrix4::Nan();

		Matrix4 m;

		float yScale = 1.0f / tan(fovRad * 0.5f);
		float xScale = yScale / aspectWiHe;

		m.rows[0].v[0] = xScale;
		m.rows[1].v[1] = yScale;
		m.rows[2].v[2] = farZ / (farZ - nearZ);
		m.rows[2].v[3] = 1.0f;
		m.rows[3].v[2] = (-nearZ * farZ) / (farZ - nearZ);

		return m;
	}

	Matrix4 Matrix4::OrthographicScreenSpace(float width, float height, float farZ, float nearZ)
	{
		Matrix4 m;

		m.rows[0].v[0] = 2.0f / width;
		m.rows[1].v[1] = 2.0f / height;
		m.rows[2].v[2] = 1.0f / (farZ - nearZ);
		m.rows[2].v[3] = nearZ / (nearZ - farZ);
		m.rows[3].v[3] = 1.0f;

		return m;
	}

	Matrix4 Matrix4::Orthographic(float aspectWiHe, float farZ, float nearZ)
	{
		Matrix4 m;

		m.rows[0].v[0] = 1.0f / aspectWiHe;
		m.rows[1].v[1] = 1.0f;
		m.rows[2].v[2] = 1.0f / (farZ - nearZ);
		m.rows[2].v[3] = nearZ / (nearZ - farZ);
		m.rows[3].v[3] = 1.0f;

		return m;
	}

	Matrix4 Matrix4::LookAt(const Vector3& up, const Vector3& at, const Vector3& position)
	{
		Matrix4 m;
		Vector3 z((at - position).Normalize());
		Vector3 x((up.Cross(z)).Normalize());
		Vector3 y(z.Cross(x));

		m.rows[0].v[0] = x.x;
		m.rows[1].v[0] = x.y;
		m.rows[2].v[0] = x.z;
		m.rows[3].v[0] = -x.Dot(position);

		m.rows[0].v[1] = y.x;
		m.rows[1].v[1] = y.y;
		m.rows[2].v[1] = y.z;
		m.rows[3].v[1] = -y.Dot(position);

		m.rows[0].v[2] = z.x;
		m.rows[1].v[2] = z.y;
		m.rows[2].v[2] = z.z;
		m.rows[3].v[2] = -z.Dot(position);

		m.rows[3].v[3] = 1.0f;

		return m;
	}



	Matrix4 Matrix4::Translation(const Vector3& translation)
	{
		Matrix4 m(Matrix4::Identity());

		m.rows[3].v[0] = translation.x;
		m.rows[3].v[1] = translation.y;
		m.rows[3].v[2] = translation.z;

		return m;
	}

	Matrix4 Matrix4::Rotation(const Vector3& axis, float angleRad)
	{
		Matrix4 r(Matrix4::Identity());
		Vector3 unitAxis(axis.UnitVector());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);
		float c = 1 - cosine;

		r.rows[0].v[0] = (unitAxis.x * unitAxis.x * c) + cosine;
		r.rows[0].v[1] = (unitAxis.x * unitAxis.y * c) - (sine * unitAxis.z);
		r.rows[0].v[2] = (unitAxis.x * unitAxis.z * c) + (sine * unitAxis.y);

		r.rows[1].v[0] = (unitAxis.y * unitAxis.x * c) + (sine * unitAxis.z);
		r.rows[1].v[1] = (unitAxis.y * unitAxis.y * c) + cosine;
		r.rows[1].v[2] = (unitAxis.y * unitAxis.z * c) - (sine * unitAxis.x);

		r.rows[2].v[0] = (unitAxis.z * unitAxis.x * c) - (sine * unitAxis.y);
		r.rows[2].v[1] = (unitAxis.z * unitAxis.y * c) + (sine * unitAxis.x);
		r.rows[2].v[2] = (unitAxis.z * unitAxis.z * c) + cosine;

		r.rows[3].v[3] = 1.0f;

		return r;
	}

	Matrix4 Matrix4::RotationX(float angleRad)
	{
		Matrix4 r(Matrix4::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[1].v[1] = cosine;
		r.rows[1].v[2] = -sine;
		r.rows[2].v[1] = sine;
		r.rows[2].v[2] = cosine;

		return r;
	}

	Matrix4 Matrix4::RotationY(float angleRad)
	{
		Matrix4 r(Matrix4::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[0].v[0] = cosine;
		r.rows[0].v[2] = sine;
		r.rows[2].v[0] = -sine;
		r.rows[2].v[2] = cosine;

		return r;
	}

	Matrix4 Matrix4::RotationZ(float angleRad)
	{
		Matrix4 r(Matrix4::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[0].v[0] = cosine;
		r.rows[0].v[1] = -sine;
		r.rows[1].v[0] = sine;
		r.rows[1].v[1] = cosine;

		return r;
	}

	Matrix4 Matrix4::Rotation(float angleRadX, float angleRadY, float angleRadZ)
	{
		return (RotationZ(angleRadZ) * RotationX(angleRadX)) * RotationY(angleRadY);
	}



	Matrix4 Matrix4::Scale(const Vector3& scale)
	{
		Matrix4 s(Matrix4::Identity());

		s.rows[0].v[0] = scale.x;
		s.rows[1].v[1] = scale.y;
		s.rows[2].v[2] = scale.z;

		return s;
	}

	Matrix4 Matrix4::Scale(float scale)
	{
		Matrix4 s(scale);
		s.m[15] = 1;

		return s;
	}
}