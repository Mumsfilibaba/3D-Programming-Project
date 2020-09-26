#include "Clock.h"

Clock::Clock()
	: m_Delta(0),
	m_Total(0),
	m_ThisTime(HighResClock::now()),
	m_LastTime(m_ThisTime)
{
}

Clock::Clock(const Clock& other)
	: m_Delta(0),
	m_Total(0),
	m_ThisTime(),
	m_LastTime()
{
	*this = other;
}



Clock& Clock::operator=(const Clock& other)
{
	if (this != &other)
	{
		m_Delta = other.m_Delta;
		m_LastTime = other.m_LastTime;
		m_ThisTime = other.m_ThisTime;
		m_Total = other.m_Total;
	}

	return *this;
}



void Clock::Tick()
{
	m_LastTime = m_ThisTime;
	m_ThisTime = HighResClock::now();
	m_Delta = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m_ThisTime - m_LastTime).count();
	m_Total += m_Delta;
}



void Clock::Reset()
{
	m_Total = 0;
	m_Delta = 0;
	m_ThisTime = HighResClock::now();
	m_LastTime = m_ThisTime;
}