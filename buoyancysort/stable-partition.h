#pragma once

#pragma once

#include <string.h>
#include <utility>
#include <vector>

namespace StablePartition
{
	// FIXME: paritition indices that are returned are buggy (and inconsistent).
	// I think the idea should be to avoid returning before_first and after_last where possible.

	template <typename Type>
	std::pair<long, long> two_way(Type *data, long before_first, long after_last, long pivot) // FIXME: I'm not doing the optimization where you can remove some of the leftmost/rightmost range
	{
		std::vector<Type> lower;
		std::vector<Type> upper;
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		//std::cout << "Partitioning on " << pivot_value << std::endl;
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] <= pivot_value)
			{
				lower.push_back(data[counter]);
			}
			else // greater elements >
			{
				upper.push_back(data[counter]);
			}
		}
		if (upper.size() == 0) // you didn't partition anything
		{
			lower.clear();
			upper.clear();
			for (int counter = left; counter <= right; counter += 1)
			{
				if (data[counter] < pivot_value)
				{
					lower.push_back(data[counter]);
				}
				else // equal elements (based on the fact nothing was partitioned) but it's technically checking for greater than or equals >=
				{
					upper.push_back(data[counter]);
				}
			}
			if (lower.size() == 0) // you didn't partition anything again, which means you can do an early return
			{
				return std::pair<long, long>(before_first, after_last); // implies there are no elements left to partition.
			}
		}
		memcpy(&data[before_first + 1], lower.data(), lower.size() * sizeof(Type)); // std::copy would be better, but I'm using raw pointers, not iterators
		memcpy(&data[(before_first + 1) + (long)lower.size() + 1], upper.data(), upper.size() * sizeof(Type));
		return std::pair<long, long>((before_first + 1) + (long)lower.size() - 1, (before_first + 1) + (long)lower.size());
	}

	template <typename Type>
	std::pair<long, long> three_way(Type *data, long before_first, long after_last, long pivot) // FIXME: I'm not doing the optimization where you can remove some of the leftmost/rightmost range
	{
		std::vector<Type> lower;
		std::vector<Type> middle;
		std::vector<Type> upper;
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		//std::cout << "Partitioning on " << pivot_value << std::endl;
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] < pivot_value)
			{
				lower.push_back(data[counter]);
			}
			else if (data[counter] == pivot_value)
			{
				middle.push_back(data[counter]);
			}
			else
			{
				upper.push_back(data[counter]);
			}
		}
		memcpy(&data[(before_first + 1)], lower.data(), lower.size() * sizeof(Type)); // std::copy would be better, but I'm using raw pointers, not iterators
		memcpy(&data[(before_first + 1) + (long)lower.size()], middle.data(), middle.size() * sizeof(Type));
		memcpy(&data[(before_first + 1) + (long)lower.size() + (long)middle.size()], upper.data(), upper.size() * sizeof(Type));
		return std::pair<long, long>((before_first + 1) + (long)lower.size() - 1, (before_first + 1) + (long)lower.size() + (long)middle.size());
	}
}