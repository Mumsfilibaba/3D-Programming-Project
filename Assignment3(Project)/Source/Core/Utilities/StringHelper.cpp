#include "StringHelper.h"
#include <string>

//Mostly because I wanna now how many chars that got traversed
//Benchmarking gives that fast_atof() takes 16% of the time that 
//std::atof does (55400ns vs 352800ns on avg) (10000 turns and 100 (tests/function)/turn)
double fast_atof(const char* const str, int& length)
{
	const char* iter = str;
	bool negative = false;
	double integer = 0.0;
	double decimal = 0.0;

	//Set length to zero
	length = 0;

	//Check sign
	if ((*iter) == '-')
	{
		negative = true;
		iter++;
		length++;
	}

	//Get "Interger"-part
	while ((*iter) > '/' && (*iter) < ':')
	{
		integer *= 10.0;
		integer += (*iter) - '0';

		length++;
		iter++;
	}

	//Get part after '.'
	if ((*iter) == '.')
	{
		iter++;
		length++;

		double base = 0.1;
		while ((*iter) > '/' && (*iter) < ':')
		{
			decimal += ((*iter) - '0') * base;
			base /= 10.0;

			length++;
			iter++;
		}
	}

	//Set sign
	if (negative)
		return -(integer + decimal);

	return integer + decimal;
}


//Mostly because I wanna now how many chars that got traversed
//Benchmarking gives that fast_atoi() takes 65% of the time that 
//std::atoi does (65000ns vs 100000ns on avg) (10000 turns and 100 (tests/function)/turn)
int fast_atoi(const char* const str, int& length)
{
	const char* iter = str;
	int integer = 0;
	bool negative = false;

	//Set length to zero
	length = 0;

	//Get sign
	if ((*iter) == '-')
	{
		length++;
		iter++;
		negative = true;
	}

	//Get Interger
	while ((*iter) > '/' && (*iter) < ':')
	{
		integer *= 10;
		integer += (*iter) - '0';

		length++;
		iter++;
	}

	if (negative)
		return -integer;

	return integer;
}

//Compare strings without taking upper or lowercase into account
bool compare_strings(const char* str, const char* str2)
{
	int counter = 0;
	char c = 0;

	while ((c = tolower(str[counter])) == tolower(str2[counter]))
	{
		if (c == '\0')
			return true;
		
		counter++;
	}

	return false;
}
