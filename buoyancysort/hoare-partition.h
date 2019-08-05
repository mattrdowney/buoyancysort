#pragma once

#include <stddef.h>

namespace HoarePartition
{
	template <typename Type>
	std::size_t partition(Type *data, long before_first, long after_last, long pivot_index)
	{
		// I would use std::partition(), but I can't count comparisons.
		long first = before_first + 1;
		long last = after_last - 1;
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