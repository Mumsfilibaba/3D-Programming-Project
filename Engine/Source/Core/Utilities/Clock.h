#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include "Time.h"

typedef std::chrono::high_resolution_clock HighResClock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

class Clock
{
public:
	Clock();
	Clock(const Clock& other);
	~Clock() {}

	Clock& operator=(const Clock& other);

	void Tick();
	void Reset();

	inline const Time& DeltaTime() const 
	{ 
		return m_Delta; 
	}

	inline const Time& TotalTime() const 
	{ 
		return m_Total; 
	}

private:
	TimePoint m_LastTime;
	TimePoint m_ThisTime;
	Time m_Delta;
	Time m_Total;
};
#endif