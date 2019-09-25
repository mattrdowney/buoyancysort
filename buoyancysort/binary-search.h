#pragma once

#include <algorithm>

namespace BinarySearch
{
	template <typename Type>
	void lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		std::lower_bound(haystack + (before_first + 1), haystack + after_last, needle);
	}

	template <typename Type>
	void upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		std::upper_bound(haystack + (before_first + 1), haystack + after_last, needle);
	}
}