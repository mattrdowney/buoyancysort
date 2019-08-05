#pragma once

#include <math.h>
#include <stddef.h>

namespace Heuristics
{
	// I refactored these functions to omit the "n * " part, since you occasionally want to do "n / " instead.
	double sqrt_log_log_n(long n)
	{
		return sqrt(log(log(n)));
	}

	double sqrt_log_n(long n)
	{
		return sqrt(log(n));
	}

	double log_log_n(long n)
	{
		return log(log(n));
	}

	double log_n(long n)
	{
		return log(n);
	}
}