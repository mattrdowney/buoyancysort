#pragma once

namespace SemiStablePartition
{
	template <typename Type>
	void partition(Type *data, long before_first, long after_last, long pivot)
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
		long right_cursor = right - greater + 1; // In theory (and almost surely in practice) this can infinite loop (when greater == 0)
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
		// FIXME: pivot needs to be in the middle and returned
	}
}