#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <string>
#include "..\..\Defines.h"

class Time
{
public:
	Time(double time_nanoseconds = 0.0);
	Time(const Time& other);
	~Time() {}

	void Set(double time_nanoseconds = 0.0);
	double AsSeconds() const;
	double AsMilliSeconds() const;
	double AsMicroSeconds() const;
	double AsNanoSeconds() const;
	Time& operator=(const Time& other);
	Time& operator+=(const Time& other);
	Time& operator-=(const Time& other);
	Time& operator=(double other);
	Time& operator+=(double other);
	Time& operator-=(double other);

	friend Time operator+(Time left, const Time& right);
	friend Time operator+(Time left, double right);
	friend Time operator-(Time left, const Time& right);
	friend Time operator-(Time left, double right);

public:
	static uint32 CurrentYear();
	static uint32 CurrentMonthOfYear();
	static uint32 CurrentDayOfMonth();
	static uint32 CurrentHour();
	static uint32 CurrentMinute();
	static Time CurrentTime();

private:
	static const tm* LocalTimestamp();
		
private:
	static std::string s_Time;

private:
	double m_NanoSeconds;
};
#endif