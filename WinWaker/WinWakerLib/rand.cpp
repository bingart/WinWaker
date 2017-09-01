
#include <time.h>
#include <stdlib.h>
#include "scrambler.h"
#include "rand.h"

double GetRandom()
{
	// Random
	srand( (int)time(0) );
	int count = rand() % 13;
	int r = rand();
	for (int i = 0; i < (count + 13); i++)
	{
		r = rand();
	}

	// shrink to range from 0 - 1
	double d = r / (RAND_MAX + 0.0);

	return d;
}