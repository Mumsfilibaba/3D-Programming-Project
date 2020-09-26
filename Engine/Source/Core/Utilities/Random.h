#ifndef RANDOM_H
#define RANDOM_H

#include "..\..\Defines.h"

class Random
{
public:
	Random() = delete;
	~Random() = delete;

	static int Int(int min, int max);
	static int Int(int min, int max, uint32 seed);
	static float Float(float min, float max);
	static float Float(float min, float max, uint32 seed);
	static double Double(double min, double max);
	static double Double(double min, double max, uint32 seed);
	static uint32 GenerateSeed();
	static void SetSeed(uint32 seed);

	static uint32 GetSeed()
	{
		return s_Seed;
	}

private:
	static uint32 s_Seed;
};

#endif