#pragma once

// The laziest way I can imagine implementing this:
// iterate [left, right] and get all elements ==pivot and partition them separately to the front or back of array.
// Do a Semi-Stable-Partition on the remainder of the array.
// Do the Reverse(Reverse(A)Reverse(B)) strategy to get a three-way partition.
namespace StableThreeWayPartition
{
	template <typename Type>
	long partition(Type *data, long before_first, long after_last, long pivot)
	{
		long left = before_first + 1;
		long right = after_last - 1;
		Type pivot_value = data[pivot];
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
		long right_pivot = right_cursor-1; // there may not be an equal pivot here yet, but it will be there at the end of both phases.
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
		long equal = 0;
		for (int counter = left; counter <= right_pivot; counter += 1)
		{
			if (data[counter] == pivot_value) // The compiler should remove the branch prediction here.
			{
				equal += 1;
			}
		}
		long less = less_than_or_equal - equal;
		left_cursor = left;
		right_cursor = right_pivot - equal + 1;
		while (right_cursor <= right_pivot) // This actually needs the 3-way partition code (not just lazy code) -- also, I think my code only works because I don't have good tests // almost surely not stable, barely a 3-way partition
		{
			while (left_cursor <= right_pivot && data[left_cursor] < pivot_value)
			{
				left_cursor += 1;
			}
			while (right_cursor <= right_pivot && data[right_cursor] == pivot_value)
			{
				right_cursor += 1;
			}
			if (left_cursor <= right_pivot && right_cursor <= right_pivot)
			{
				std::swap(data[left_cursor], data[right_cursor]);
				left_cursor += 1;
				right_cursor += 1;
			}
		}
		long left_pivot = right_cursor;
		return right_pivot;
	}
}