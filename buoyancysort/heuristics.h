#pragma once

#include <math.h>
#include <stddef.h>

namespace Heuristics
{
	double sqrt_log_log(std::size_t n)
	{
		return sqrt(log(log(n)));
	}
}