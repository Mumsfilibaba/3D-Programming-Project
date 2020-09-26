#include "Random.h"
#include <ctime>
#include <cstdlib>

uint32 Random::s_Seed = 0;



int Random::Int(int min, int max)
{
	return (min < max) ? min + (rand() % (max - min + 1)) : 0;
}

int Random::Int(int min, int max, uint32 seed)
{
	srand(seed);

	int result = Int(min, max);

	srand(s_Seed);

	return result;
}



float Random::Float(float min, float max)
{
	return min + (((float)rand() / (float)RAND_MAX) * (max - min));
}

float Random::Float(float min, float max, uint32 seed)
{
	srand(seed);

	float result = Float(min, max);

	srand(s_Seed);

	return result;
}



double Random::Double(double min, double max)
{
	return min + (((double)rand() / (double)RAND_MAX) * (max - min));
}

double Random::Double(double min, double max, uint32 seed)
{
	srand(seed);

	double result = Double(min, max);

	srand(s_Seed);

	return result;
}



uint32 Random::GenerateSeed()
{
	s_Seed = (uint32)time(0);

	srand(s_Seed);

	return s_Seed;
}



void Random::SetSeed(uint32 seed)
{
	s_Seed = seed;

	srand(s_Seed);
}