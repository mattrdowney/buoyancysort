#pragma once

#include <stddef.h>

namespace HoarePartition
{
	template <typename Type>
	std::size_t partition(Type *data, std::size_t first, std::size_t after_last, std::size_t pivot_index)
	{
		// I would use std::partition(), but I can't count comparisons.
		std::size_t last = after_last - 1;
		Type pivot = data[pivot_index];
		while (true)
		{
			while (data[first] < pivot)
			{
				first += 1;
			}
			while (pivot < data[last])
			{
				last -= 1;
			}
			if (first >= last)
			{
				return last;
			}
			std::swap(data[first], data[last]);
			first += 1;
			last -= 1;
		}
	}
}