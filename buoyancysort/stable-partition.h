#pragma once

#pragma once

#include <string.h>
#include <utility>
#include <vector>

namespace StablePartition
{
	template <typename Type>
	long two_way(Type *data, long before_first, long after_last, long pivot)
	{
		std::vector<Type> lower;
		std::vector<Type> upper;
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] < pivot_value)
			{
				lower.push_back(data[counter]);
			}
			else if(data[counter] > pivot_value) // Stable two-way partition uses just as many comparisons (you might as well do a three-way partition)
			{
				upper.push_back(data[counter]);
			}
			else if (counter < pivot)
			{
				lower.push_back(data[counter]);
			}
			else if (counter > pivot)
			{
				upper.push_back(data[counter]);
			}
		}
		memcpy(&data[before_first + 1], lower.data(), lower.size() * sizeof(Type)); // std::copy would be better, but I'm using raw pointers, not iterators
		data[(before_first + 1) + (long)lower.size()] = pivot_value;
		memcpy(&data[(before_first + 1) + (long)lower.size() + 1], upper.data(), upper.size() * sizeof(Type));
		return (before_first + 1) + (long)lower.size();
	}

	template <typename Type>
	std::pair<long, long> three_way(Type *data, long before_first, long after_last, long pivot)
	{
		std::vector<Type> lower;
		std::vector<Type> middle;
		std::vector<Type> upper;
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
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
		return std::pair<long, long>((before_first + 1) + (long)lower.size(), (before_first + 1) + (long)lower.size() + (long)middle.size() - 1);
	}
}