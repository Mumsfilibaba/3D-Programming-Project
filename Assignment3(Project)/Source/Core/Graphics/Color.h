#ifndef COLOR_H
#define COLOR_H

#include "ColorF.h"

class Color
{
public:
	Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 0);
	Color(const Color& other);
	explicit Color(const ColorF& colorf);
	explicit Color(const Math::Vector4& vector4);
	~Color() {}

	Color& Add(const Color& other);
	Color& Add(unsigned char scalar);
	Color& Subtract(const Color& other);
	Color& Subtract(unsigned char scalar);
	Color& Multiply(const Color& other);
	Color& Multiply(unsigned char scalar);
	Color& Divide(const Color& other);
	Color& Divide(unsigned char scalar);

	bool Equals(const Color& other) const;

	std::string ToString() const;

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	Color& operator=(const Math::Vector4& other);
	Color& operator=(const ColorF& other);
	Color& operator=(const Color& other);
	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);
	Color& operator*=(const Color& other);
	Color& operator/=(const Color& other);
	Color& operator+=(unsigned char scalar);
	Color& operator-=(unsigned char scalar);
	Color& operator*=(unsigned char scalar);
	Color& operator/=(unsigned char scalar);

	explicit operator ColorF();


	friend Color operator-(Color left, const Color& right);
	friend Color operator+(Color left, const Color& right);
	friend Color operator*(Color left, const Color& right);
	friend Color operator/(Color left, const Color& right);
	friend Color operator-(Color left, unsigned char right);
	friend Color operator+(Color left, unsigned char right);
	friend Color operator*(Color left, unsigned char right);
	friend Color operator/(Color left, unsigned char right);
	friend Color operator*(unsigned char left, Color right);

public:
	union 
	{
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};

		unsigned char rgba[4];
		unsigned int color;
	};

public:
	const static Color BLACK;
	const static Color WHITE;
	const static Color WARMWHITE;
	const static Color GRAY;
	const static Color DARKGRAY;
	const static Color SOFTGRAY;
	const static Color RED;
	const static Color DARKRED;
	const static Color SOFTRED;
	const static Color GREEN;
	const static Color DARKGREEN;
	const static Color SOFTGREEN;
	const static Color BLUE;
	const static Color DARKBLUE;
	const static Color SOFTBLUE;
	const static Color CORNFLOWERBLUE;
	const static Color YELLOW;
	const static Color DARKYELLOW;
	const static Color SOFTYELLOW;
	const static Color MAGENTA;
	const static Color PURPLE;
	const static Color PINK;
};
#endif