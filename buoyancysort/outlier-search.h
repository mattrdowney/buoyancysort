#pragma once

namespace OutlierSearch
{
	// You can think of this as a specialized BinarySearch. It's good at finding elements at the extremes but performs about half as well towards the middle.
	// It's similar to Timsort's gallop mode (but it works in both directions: optimizing for both random and nearly-sorted data).
	// To be clear, it has disadvantages on random data as-written. E.g. you basically need a random access for the middle value which is very bad (and it's also one more comparison per each of n- merges which might be problematic with e.g. strings).
	template <typename Type>
	void lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		// I really just -don't- wanna work on this right now.
		// Even if I can use gallop_upper_bound() as a template from here and add almost no new code: https://github.com/tvanslyke/timsort-cpp/blob/master/include/tim/utils.h
	}

	template <typename Type>
	void upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{

	}
}