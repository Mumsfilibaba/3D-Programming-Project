#ifndef POINT_H
#define POINT_H

#include <string>
#include "..\..\Defines.h"

namespace Math
{
	class Point
	{
	public:
		Point(int x = 0, int y = 0);
		Point(const Point& other);
		~Point() {}

		Point& Add(const Point& other);
		Point& Add(int scalar);
		Point& Subtract(const Point& other);
		Point& Subtract(int scalar);
		Point& Multiply(int scalar);
		Point& Divide(int scalar);

		bool Equals(const Point& other) const;

		std::string ToString() const;

		Point& operator=(const Point& right);
		Point& operator+=(const Point& right);
		Point& operator-=(const Point& right);
		Point& operator+=(int right);
		Point& operator-=(int right);
		Point& operator*=(int right);
		Point& operator/=(int right);

		bool operator==(const Point& other) const;
		bool operator!=(const Point& other) const;


		friend Point operator+(Point left, const Point& right);
		friend Point operator-(Point left, const Point& right);
		friend Point operator+(Point left, int right);
		friend Point operator-(Point left, int right);
		friend Point operator*(Point left, int right);
		friend Point operator/(Point left, int right);
		friend Point operator*(int left, Point right);

	public:
		int x;
		int y;
	};
}
#endif