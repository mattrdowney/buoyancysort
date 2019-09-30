#pragma once

#include <algorithm>
#include "gallop-search.h"

// I knew I was gonna hate testing these functions before I wrote them DX
namespace OutlierSearch
{
	// You can think of this as a specialized BinarySearch. It's good at finding elements at the extremes but performs about half as well towards the middle.
	// It's similar to Timsort's gallop mode (but it works in both directions: optimizing for both random and nearly-sorted data).
	// To be clear, it has disadvantages on random data as-written. E.g. you basically need a random access for the middle value which is very bad (and it's also one more comparison per each of n- merges which might be problematic with e.g. strings).
	template <typename Type>
	long lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		if (before_first + 1 < after_last) // if there is at least one element
		{
			long median = (before_first + after_last) / 2; // CONSIDER: proper (low + (high - low)/2) trapezoid rule.
			if (haystack[median] < needle)
			{
				return GallopSearch::reverse_lower_bound(haystack, median - 1, after_last, needle);
			}
			else
			{
				return GallopSearch::forward_lower_bound(haystack, before_first, median + 1, needle);
			}
		}
		return after_last;
	}

	template <typename Type>
	long upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		if (before_first + 1 < after_last) // if there is at least one element
		{
			long median = (before_first + after_last) / 2; // CONSIDER: proper (low + (high - low)/2) trapezoid rule.
			if (haystack[median] <= needle)
			{
				return GallopSearch::reverse_upper_bound(haystack, median - 1, after_last, needle);
			}
			else
			{
				return GallopSearch::forward_upper_bound(haystack, before_first, median + 1, needle);
			}
		}
		return before_first;
	}
}