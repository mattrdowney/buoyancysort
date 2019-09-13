#pragma once

#include <utility>

namespace SemiStablePartition // Semi-Stable is very misleading. Originally I thought the partition was stable when the pivot was unique but this is not the case. Additionally, both sides are usually disrupted / unstable.
{
	template <typename Type>
	std::pair<long, long> two_way(Type *data, long before_first, long after_last, long pivot)
	{
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		//std::cout << "Partitioning on " << pivot_value << std::endl;
		// Ignore trivially-partitioned elements
		// In the context of Buoyancysort/"Spindles" this is a great optimization
		while (data[left] < pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			left += 1;
		}
		while (data[right] > pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			right -= 1;
		}
		long size = right - left + 1;
		long greater = 0;
		// FIXME: cache-coherency between this step and the next step
		// This step is super cheap (and it's not always required for 100% of elements)
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] > pivot_value) // The compiler should remove the branch prediction here.
			{
				greater += 1;
			}
		}
		if (greater > 0)
		{
			long less_than_or_equal = size - greater;
			long left_cursor = left;
			long right_cursor = right - greater + 1;
			while (right_cursor <= right)
			{
				while (left_cursor <= right && data[left_cursor] <= pivot_value)
				{
					left_cursor += 1;
				}
				while (right_cursor <= right && data[right_cursor] > pivot_value)
				{
					right_cursor += 1;
				}
				if (left_cursor <= right && right_cursor <= right)
				{
					std::swap(data[left_cursor], data[right_cursor]);
					left_cursor += 1;
					right_cursor += 1;
				}
			}
			return std::pair<long, long>(right - greater, right - greater + 1);
		}
		else // no greater elements to partition
		{
			long lesser = 0;
			for (int counter = left; counter <= right; counter += 1)
			{
				if (data[counter] < pivot_value) // The compiler should remove the branch prediction here.
				{
					lesser += 1;
				}
			}
			if (lesser > 0)
			{
				long less_than_or_equal = size - lesser;
				long left_cursor = left;
				long right_cursor = right - lesser + 1;
				while (right_cursor <= right)
				{
					while (left_cursor <= right && data[left_cursor] < pivot_value)
					{
						left_cursor += 1;
					}
					while (right_cursor <= right && data[right_cursor] >= pivot_value)
					{
						right_cursor += 1;
					}
					if (left_cursor <= right && right_cursor <= right)
					{
						std::swap(data[left_cursor], data[right_cursor]);
						left_cursor += 1;
						right_cursor += 1;
					}
				}
				return std::pair<long, long>(right - lesser, right - lesser + 1);
			}
			else // you didn't partition anything again, which means you can do an early return
			{
				return std::pair<long, long>(before_first, after_last); // implies there are no elements left to partition.
			}
		}
	}

	template <typename Type>
	std::pair<long, long> three_way(Type *data, long before_first, long after_last, long pivot) // not moving / swapping elements helps with performance, cache-coherency also helps with performance (so in practical terms this is more useful than stable partitioning)
	{
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		//std::cout << "Partitioning on " << pivot_value << std::endl;
		// FIXME? (I don't think this is easy to fix?): if you partition an array that's 50% redundant values on a unique value, then one side gets fucked up.// Confirmed: this should be the intended behavior.
		// Ignore trivially-partitioned elements
		// In the context of Buoyancysort/"Spindles" this is a great optimization
		while (data[left] < pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			left += 1;
		}
		while (data[right] > pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			right -= 1;
		}
		long size = right - left + 1;
		long greater = 0;
		// FIXME: cache-coherency between this step and the next step
		// This step is super cheap (and it's not always required for 100% of elements)
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] > pivot_value) // The compiler should remove the branch prediction here.
			{
				greater += 1;
			}
		}
		long less_than_or_equal = size - greater;
		long left_cursor = left;
		long right_cursor = right - greater + 1;
		long right_pivot = right_cursor - 1; // there may not be an equal pivot here yet, but it will be there at the end of both phases.
		while (right_cursor <= right)
		{
			while (left_cursor <= right && data[left_cursor] <= pivot_value)
			{
				left_cursor += 1;
			}
			while (right_cursor <= right && data[right_cursor] > pivot_value)
			{
				right_cursor += 1;
			}
			if (left_cursor <= right && right_cursor <= right)
			{
				std::swap(data[left_cursor], data[right_cursor]);
				left_cursor += 1;
				right_cursor += 1;
			}
		}
		right = right_pivot;
		left_cursor = left;
		for (right_cursor = left; right_cursor <= right; right_cursor += 1)
		{
			if (data[right_cursor] < pivot_value)
			{
				std::swap(data[left_cursor], data[right_cursor]);
				left_cursor += 1;
			}
		}
		left = left_cursor - 1;
		return std::pair<long, long>(left, right);
	}
}