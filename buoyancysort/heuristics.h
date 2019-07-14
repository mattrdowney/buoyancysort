#pragma once

#include <math.h>
#include <stddef.h>

namespace Heuristics
{
	double n_sqrt_log_log(std::size_t n)
	{
		return n * sqrt(log(log(n)));
	}

	double n_sqrt_log(std::size_t n)
	{
		return n * sqrt(log(n));
	}

	double n_log_log(std::size_t n)
	{
		return n * log(log(n));
	}

	double n_log(std::size_t n)
	{
		return n * log(n);
	}
}