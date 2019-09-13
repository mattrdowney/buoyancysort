#pragma once

#include <algorithm>
#include <utility>

namespace UnstablePartition
{
	template <typename Type>
	std::pair<long, long> hoare(Type *data, long before_first, long after_last, long pivot)
	{
		long left = before_first;
		long right = after_last;
		Type pivot_value = data[pivot];
		std::cout << "Partitioning on " << pivot_value << std::endl;

		while (true)
		{
			do
			{
				left += 1;
			} while (data[left] < pivot_value);
			do
			{
				right -= 1;
			} while (data[right] > pivot_value);

			if (left >= right)
			{
				return std::pair<long, long>(right, right);
			}
			std::swap(data[left], data[right]);
		}
	}

	template <typename Type>
	std::pair<long, long> lomuto(Type *data, long before_first, long after_last, long pivot)
	{
		long left = before_first + 1;
		Type pivot_value = data[pivot];
		std::cout << "Partitioning on " << pivot_value << std::endl;

		for (long right = left; right < after_last; right += 1)
		{
			if (data[right] < pivot)
			{
				std::swap(data[left], data[right]);
				left += 1;
			}
		}
		return std::pair<long, long>(left + 1, left); // return elements one-past the end; I think this is consistent (e.g. for when partitions have no values)
	}
}