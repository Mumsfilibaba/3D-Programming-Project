#include "Matrix3.h"

namespace Math
{
	Matrix3::Matrix3()
	{
		ZeroBlock(sizeof(float) * 9, &m);
	}

	Matrix3::Matrix3(float diagonal)
	{
		ZeroBlock(sizeof(float) * 9, &m);

		rows[0].v[0] = diagonal;
		rows[1].v[1] = diagonal;
		rows[2].v[2] = diagonal;
	}

	Matrix3::Matrix3(const Vector3& r1, const Vector3& r2, const Vector3& r3)
	{
		rows[0] = r1;
		rows[1] = r2;
		rows[2] = r3;
	}

	Matrix3::Matrix3(const Matrix3& other)
	{
		memcpy(m, other.m, sizeof(float) * 9);
	}



	Vector3 Matrix3::Multiply(const Vector3& vector) const
	{
		float t[3];

		t[0] = (rows[0].v[0] * vector.v[0]) + (rows[0].v[1] * vector.v[1]) + (rows[0].v[2] * vector.v[2]);

		t[1] = (rows[1].v[0] * vector.v[0]) + (rows[1].v[1] * vector.v[1]) + (rows[1].v[2] * vector.v[2]);

		t[2] = (rows[2].v[0] * vector.v[0]) + (rows[2].v[1] * vector.v[1]) + (rows[2].v[2] * vector.v[2]);

		return Vector3(t[0], t[1], t[2]);
	}

	Matrix3& Matrix3::Multiply(const Matrix3& other)
	{
		float t[9];

		t[0] = (rows[0].v[0] * other.rows[0].v[0]) + (rows[0].v[1] * other.rows[1].v[0]) + (rows[0].v[2] * other.rows[2].v[0]);
		t[1] = (rows[0].v[0] * other.rows[0].v[1]) + (rows[0].v[1] * other.rows[1].v[1]) + (rows[0].v[2] * other.rows[2].v[1]);
		t[2] = (rows[0].v[0] * other.rows[0].v[2]) + (rows[0].v[1] * other.rows[1].v[2]) + (rows[0].v[2] * other.rows[2].v[2]);

		t[3] = (rows[1].v[0] * other.rows[0].v[0]) + (rows[1].v[1] * other.rows[1].v[0]) + (rows[1].v[2] * other.rows[2].v[0]);
		t[4] = (rows[1].v[0] * other.rows[0].v[1]) + (rows[1].v[1] * other.rows[1].v[1]) + (rows[1].v[2] * other.rows[2].v[1]);
		t[5] = (rows[1].v[0] * other.rows[0].v[2]) + (rows[1].v[1] * other.rows[1].v[2]) + (rows[1].v[2] * other.rows[2].v[2]);

		t[6] = (rows[2].v[0] * other.rows[0].v[0]) + (rows[2].v[1] * other.rows[1].v[0]) + (rows[2].v[2] * other.rows[2].v[0]);
		t[7] = (rows[2].v[0] * other.rows[0].v[1]) + (rows[2].v[1] * other.rows[1].v[1]) + (rows[2].v[2] * other.rows[2].v[1]);
		t[8] = (rows[2].v[0] * other.rows[0].v[2]) + (rows[2].v[1] * other.rows[1].v[2]) + (rows[2].v[2] * other.rows[2].v[2]);


		memcpy(m, t, sizeof(float) * 9);

		return *this;
	}

	Matrix3& Matrix3::Multiply(float scalar)
	{
		for (char i = 0; i < 9; i++)
			m[i] *= scalar;

		return *this;
	}



	Matrix3& Matrix3::Add(const Matrix3& other)
	{
		for (char i = 0; i < 9; i++)
			m[i] += other.m[i];

		return *this;
	}

	Matrix3& Matrix3::Add(float scalar)
	{
		for (char i = 0; i < 9; i++)
			m[i] += scalar;

		return *this;
	}



	Matrix3& Matrix3::Subtract(const Matrix3& other)
	{
		for (char i = 0; i < 9; i++)
			m[i] -= other.m[i];

		return *this;
	}

	Matrix3& Matrix3::Subtract(float scalar)
	{
		for (char i = 0; i < 9; i++)
			m[i] -= scalar;

		return *this;
	}



	Matrix3& Matrix3::Divide(float scalar)
	{
		for (char i = 0; i < 9; i++)
			m[i] /= scalar;

		return *this;
	}



	bool Matrix3::Equals(const Matrix3& other) const
	{
		for (char i = 0; i < 9; i++)
		{
			if (m[i] != other.m[i])
				return false;
		}

		return true;
	}



	float* Matrix3::GetArray()
	{
		return m;
	}

	const float* Matrix3::GetArray() const
	{
		return m;
	}

	float Matrix3::GetElement(uint8 r, uint8 c) const
	{
		assert(r < 3);
		assert(c < 3);

		return rows[r].v[c];
	}

	float& Matrix3::GetElement(uint8 r, uint8 c)
	{
		assert(r < 3);
		assert(c < 3);

		return rows[r].v[c];
	}



	Vector3 Matrix3::GetRow(uint8 r) const
	{
		assert(r < 3);

		return rows[r];
	}

	Vector3& Matrix3::GetRow(uint8 r)
	{
		assert(r < 3);

		return rows[r];
	}



	Matrix3& Matrix3::Transpose()
	{
		float temp[9];

		temp[0] = rows[0].v[0];
		temp[1] = rows[1].v[0];
		temp[2] = rows[2].v[0];

		temp[3] = rows[0].v[1];
		temp[4] = rows[1].v[1];
		temp[5] = rows[2].v[1];

		temp[6] = rows[0].v[2];
		temp[7] = rows[1].v[2];
		temp[8] = rows[2].v[2];

		memcpy(m, temp, sizeof(float) * 9);

		return *this;
	}

	Matrix3 Matrix3::TransposeOf() const
	{
		return Matrix3(*this).Transpose();
	}



	Matrix3& Matrix3::Invert()
	{
		float det = Determinant();

		if (det == 0.0f)
			*this = Matrix3::Nan();
		else
			*this = (Adjugate() / det);

		return*this;
	}

	Matrix3 Matrix3::InverseOf() const
	{
		return Matrix3(*this).Invert();
	}



	Matrix3 Matrix3::Cofactor() const
	{
		return Adjugate().Transpose();
	}

	Matrix3 Matrix3::Adjugate() const
	{
		Matrix3 adj;

		//d11 - d12 - d13
		adj.rows[0].v[0] = ((rows[1].v[1] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[1]));
		adj.rows[1].v[0] = -((rows[1].v[0] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[0]));
		adj.rows[2].v[0] = ((rows[1].v[0] * rows[2].v[1]) - (rows[1].v[1] * rows[2].v[0]));

		//d21 - d22 - 23
		adj.rows[0].v[1] = -((rows[0].v[1] * rows[2].v[2]) - (rows[0].v[2] * rows[2].v[1]));
		adj.rows[1].v[1] = ((rows[0].v[0] * rows[2].v[2]) - (rows[0].v[2] * rows[2].v[0]));
		adj.rows[2].v[1] = -((rows[0].v[0] * rows[2].v[1]) - (rows[0].v[1] * rows[2].v[0]));

		//d31 - 32 - 33
		adj.rows[0].v[2] = ((rows[0].v[1] * rows[1].v[2]) - (rows[0].v[2] * rows[1].v[1]));
		adj.rows[1].v[2] = -((rows[0].v[0] * rows[1].v[2]) - (rows[0].v[2] * rows[1].v[0]));
		adj.rows[2].v[2] = ((rows[0].v[0] * rows[1].v[1]) - (rows[0].v[1] * rows[1].v[0]));

		return adj;
	}



	float Matrix3::Determinant() const
	{
		float det = 0;

		det = (rows[0].v[0] * ((rows[1].v[1] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[1])));
		det -= (rows[0].v[1] * ((rows[1].v[0] * rows[2].v[2]) - (rows[1].v[2] * rows[2].v[0])));
		det += (rows[0].v[2] * ((rows[1].v[0] * rows[2].v[1]) - (rows[1].v[1] * rows[2].v[0])));

		return det;
	}



	std::string Matrix3::ToString() const
	{
		return std::string(rows[0].ToString() + '\n' + rows[1].ToString() + '\n' + rows[2].ToString());
	}



	bool Matrix3::operator==(const Matrix3& other) const
	{
		return Equals(other);
	}

	bool Matrix3::operator!=(const Matrix3& other) const
	{
		return !Equals(other);
	}



	float Matrix3::operator()(uint32 r, uint32 c) const
	{
		return GetElement(r, c);
	}

	float& Matrix3::operator()(uint32 r, uint32 c)
	{
		return GetElement(r, c);
	}

	Vector3 Matrix3::operator()(uint32 r) const
	{
		return GetRow(r);
	}

	Vector3& Matrix3::operator()(uint32 r)
	{
		return GetRow(r);
	}



	Vector3& operator*=(Vector3& left, const Matrix3& right)
	{
		return left = right.Multiply(left);
	}

	Vector3 operator*(Vector3 left, const Matrix3& right)
	{
		return right.Multiply(left);
	}



	Matrix3 operator+(Matrix3 left, const Matrix3& right)
	{
		return left.Add(right);
	}

	Matrix3 operator-(Matrix3 left, const Matrix3& right)
	{
		return left.Subtract(right);
	}

	Matrix3 operator*(Matrix3 left, const Matrix3& right)
	{
		return left.Multiply(right);
	}



	Matrix3 operator+(Matrix3 left, float right)
	{
		return left.Add(right);
	}

	Matrix3 operator-(Matrix3 left, float right)
	{
		return left.Subtract(right);
	}

	Matrix3 operator*(Matrix3 left, float right)
	{
		return left.Multiply(right);
	}

	Matrix3 operator/(Matrix3 left, float right)
	{
		return left.Divide(right);
	}

	Matrix3 operator*(float left, Matrix3 right)
	{
		return right.Multiply(left);
	}

	Matrix3 operator/(float left, Matrix3 right)
	{
		return right.Divide(left);
	}



	Matrix3& Matrix3::operator=(const Matrix3& other)
	{
		if (this != &other)
			memcpy(m, other.m, sizeof(float) * 9);

		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& right)
	{
		return Add(right);
	}

	Matrix3& Matrix3::operator-=(const Matrix3& right)
	{
		return Subtract(right);
	}

	Matrix3& Matrix3::operator*=(const Matrix3& right)
	{
		return Multiply(right);
	}



	Matrix3& Matrix3::operator+=(float right)
	{
		return Add(right);
	}

	Matrix3& Matrix3::operator-=(float right)
	{
		return Subtract(right);
	}

	Matrix3& Matrix3::operator*=(float right)
	{
		return Multiply(right);
	}

	Matrix3& Matrix3::operator/=(float right)
	{
		return Divide(right);
	}



	Matrix3 Matrix3::Nan()
	{
		return Matrix3(Vector3::Nan(), Vector3::Nan(), Vector3::Nan());
	}

	Matrix3 Matrix3::Identity()
	{
		return Matrix3(1);
	}



	Matrix3 Matrix3::Translation(const Vector2& translation)
	{
		Matrix3 t(Matrix3::Identity());

		t.rows[2].v[0] = translation.x;
		t.rows[2].v[1] = translation.y;

		return t;
	}

	Matrix3 Matrix3::Rotation(const Vector3& axis, float angleRad)
	{
		Matrix3 r(Matrix3::Identity());
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

		return r;
	}

	Matrix3 Matrix3::RotationX(float angleRad)
	{
		Matrix3 r(Matrix3::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[1].v[1] = cosine;
		r.rows[1].v[2] = -sine;

		r.rows[2].v[1] = sine;
		r.rows[2].v[2] = cosine;

		return r;
	}

	Matrix3 Matrix3::RotationY(float angleRad)
	{
		Matrix3 r(Matrix3::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[0].v[0] = cosine;
		r.rows[0].v[2] = sine;
		r.rows[2].v[0] = -sine;
		r.rows[2].v[2] = cosine;

		return r;
	}

	Matrix3 Matrix3::RotationZ(float angleRad)
	{
		Matrix3 r(Matrix3::Identity());

		float cosine = cosf(angleRad);
		float sine = sinf(angleRad);

		r.rows[0].v[0] = cosine;
		r.rows[0].v[1] = -sine;
		r.rows[1].v[0] = sine;
		r.rows[1].v[1] = cosine;

		return r;
	}

	Matrix3 Matrix3::Rotation(float angleRadZ, float angleRadX, float angleRadY)
	{
		return (RotationZ(angleRadZ) * RotationX(angleRadX)) * RotationY(angleRadY);
	}

	Matrix3 Matrix3::Scale(const Vector3& scale)
	{
		Matrix3 s;

		s.rows[0].v[0] = scale.x;
		s.rows[1].v[1] = scale.y;
		s.rows[2].v[2] = scale.z;

		return s;
	}

	Matrix3 Matrix3::Scale(float scale)
	{
		return Matrix3(scale);
	}
}