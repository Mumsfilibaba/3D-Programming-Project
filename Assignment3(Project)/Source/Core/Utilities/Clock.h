#pragma once

#include <chrono>
#include "Time.h"

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
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	TimePoint m_LastTime;
	TimePoint m_ThisTime;
	Time m_Delta;
	Time m_Total;
};