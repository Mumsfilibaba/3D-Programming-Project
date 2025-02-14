#ifndef COLORF_H
#define COLORF_H

#include "..\Math\Vector4.h"

class ColorF
{
public:
	ColorF(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
	ColorF(const ColorF& other);
	explicit ColorF(const Math::Vector4& vector4);
	~ColorF() {}

	ColorF& Add(const ColorF& other);
	ColorF& Add(float scalar);
	ColorF& Subtract(const ColorF& other);
	ColorF& Subtract(float scalar);
	ColorF& Multiply(const ColorF& other);
	ColorF& Multiply(float scalar);
	ColorF& Divide(const ColorF& other);
	ColorF& Divide(float scalar);

	ColorF& Lerp(const ColorF& src, float delta);
	ColorF Lerp(const ColorF& src, float delta) const;

	bool Equals(const ColorF& other) const;

	std::string ToString() const;

	bool operator==(const ColorF& other) const;
	bool operator!=(const ColorF& other) const;

	ColorF& operator=(const Math::Vector4& other);
	ColorF& operator=(const ColorF& other);
	ColorF& operator+=(const ColorF& other);
	ColorF& operator-=(const ColorF& other);
	ColorF& operator*=(const ColorF& other);
	ColorF& operator/=(const ColorF& other);
	ColorF& operator+=(float scalar);
	ColorF& operator-=(float scalar);
	ColorF& operator*=(float scalar);
	ColorF& operator/=(float scalar);

	operator const float*() const;
	operator float*();
	explicit operator Math::Vector4&();
	explicit operator const Math::Vector4&() const;


	friend ColorF operator-(ColorF left, const ColorF& right);
	friend ColorF operator+(ColorF left, const ColorF& right);
	friend ColorF operator*(ColorF left, const ColorF& right);
	friend ColorF operator/(ColorF left, const ColorF& right);
	friend ColorF operator-(ColorF left, float right);
	friend ColorF operator+(ColorF left, float right);
	friend ColorF operator*(ColorF left, float right);
	friend ColorF operator/(ColorF left, float right);
	friend ColorF operator*(float left, ColorF right);

private:
	void Abs();

public:
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float rgba[4];
		Math::Vector4 vector;
	};

public:
	const static ColorF BLACK;
	const static ColorF WHITE;
	const static ColorF GRAY;
	const static ColorF DARKGRAY;
	const static ColorF SOFTGRAY;
	const static ColorF RED;
	const static ColorF DARKRED;
	const static ColorF SOFTRED;
	const static ColorF GREEN;
	const static ColorF DARKGREEN;
	const static ColorF SOFTGREEN;
	const static ColorF BLUE;
	const static ColorF DARKBLUE;
	const static ColorF SOFTBLUE;
	const static ColorF CORNFLOWERBLUE;
	const static ColorF YELLOW;
	const static ColorF DARKYELLOW;
	const static ColorF SOFTYELLOW;
	const static ColorF MAGENTA;
	const static ColorF PURPLE;
	const static ColorF PINK;
};
#endif