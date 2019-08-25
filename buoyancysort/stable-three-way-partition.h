#pragma once

// The laziest way I can imagine implementing this:
// iterate [left, right] and get all elements ==pivot and partition them separately to the front or back of array.
// Do a Semi-Stable-Partition on the remainder of the array.
// Do the Reverse(Reverse(A)Reverse(B)) strategy to get a three-way partition.
namespace StableThreeWayPartition
{
	template <typename Type>
	void partition(Type *data, long before_first, long after_last, long pivot)
	{
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
		// Ignore trivially-partitioned elements
		while (data[left] < pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			left += 1;
		}
		while (data[right] > pivot_value) // This is only safe if the pivot exists, since it becomes a sentinel
		{
			right -= 1;
		}
		long size = right - left + 1;
		long less = 0;
		long greater = 0;
		for (int counter = left; counter <= right; counter += 1)
		{
			if (data[counter] < pivot_value)
			{
				less += 1;
			}
			if (data[counter] > pivot_value)
			{
				greater += 1;
			}
		}
		long left_equal = 0;
		for (int counter = left; counter < left + less; counter += 1)
		{
			if (data[counter] == pivot_value)
			{
				left_equal += 1;
			}
		}
		long right_equal = 0;
		for (int counter = right - greater + 1; counter <= right; counter += 1)
		{
			if (data[counter] == pivot_value)
			{
				right_equal += 1;
			}
		}
	}
}