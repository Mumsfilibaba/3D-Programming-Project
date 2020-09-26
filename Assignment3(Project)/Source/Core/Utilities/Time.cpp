#include "Time.h"

Time::Time(double time_nanoseconds)
	: m_NanoSeconds(abs(time_nanoseconds))
{
}

Time::Time(const Time& other)
	: m_NanoSeconds()
{
	*this = other;
}



void Time::Set(double time_nanoseconds)
{
	m_NanoSeconds = time_nanoseconds;
}



double Time::AsSeconds() const
{
	return AsMilliSeconds() / 1000.0;
}

double Time::AsMilliSeconds() const
{
	return AsMicroSeconds() / 1000.0;
}

double Time::AsMicroSeconds() const
{
	return AsNanoSeconds() / 1000.0;
}

double Time::AsNanoSeconds() const
{
	return m_NanoSeconds;
}



Time& Time::operator=(const Time& other)
{
	m_NanoSeconds = other.m_NanoSeconds;
	return *this;
}

Time& Time::operator=(double other)
{
	m_NanoSeconds = other;
	return *this;
}

Time& Time::operator+=(const Time& other)
{
	return (*this = (*this + other));
}

Time& Time::operator-=(const Time& other)
{
	return (*this = (*this - other));
}

Time& Time::operator+=(double other)
{
	return (*this = (*this + other));
}

Time& Time::operator-=(double other)
{
	return (*this = (*this - other));
}



const tm* Time::LocalTimestamp()
{
	const time_t now = time(0);
	return localtime(&now);
}



Time operator+(Time left, const Time& right)
{
	left.m_NanoSeconds += right.m_NanoSeconds;
	return left;
}

Time operator-(Time left, const Time& right)
{
	left.m_NanoSeconds -= right.m_NanoSeconds;
	return left;
}



Time operator+(Time left, double right)
{
	left.m_NanoSeconds += right;
	return left;
}

Time operator-(Time left, double right)
{
	left.m_NanoSeconds -= right;
	return left;
}



uint32 Time::CurrentYear()
{
	return (1900 + LocalTimestamp()->tm_year);
}

uint32 Time::CurrentMonthOfYear()
{
	return (1 + LocalTimestamp()->tm_mon);
}

uint32 Time::CurrentDayOfMonth()
{
	return (LocalTimestamp()->tm_mday);
}

uint32 Time::CurrentHour()
{
	return (LocalTimestamp()->tm_hour);
}

uint32 Time::CurrentMinute()
{
	return (LocalTimestamp()->tm_min);
}

Time Time::CurrentTime()
{
	Time t;

	t.m_NanoSeconds = (LocalTimestamp()->tm_sec * 1000.0 * 1000.0 * 1000.0);

	return t;
}