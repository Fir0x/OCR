#include "random.h"
#include <stdio.h>

size_t Random_size(size_t min, size_t max)
{
	return (size_t)(rand() / (double) RAND_MAX * (max - min) + min); //Uniform formula
}

double Random_double(double min, double max)
{
	return (double)(rand() / (double)RAND_MAX * (max - min) + min);
}
