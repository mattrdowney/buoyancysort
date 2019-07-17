#pragma once

#include <stddef.h>

namespace PowerOfTwo
{
	std::size_t upper_power_of_two(std::size_t number)
	{
		number--;
		number |= number >> 1;
		number |= number >> 2;
		number |= number >> 4;
		number |= number >> 8;
		number |= number >> 16;
		number++;
		return number;
	}

	std::size_t lower_power_of_two(std::size_t number)
	{
		std::size_t upper_power = upper_power_of_two(number);
		if (number == upper_power)
		{
			return number;
		}
		return upper_power / 2;
	}
}