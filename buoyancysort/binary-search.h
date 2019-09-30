#pragma once

#include <algorithm>

namespace BinarySearch
{
	template <typename Type>
	long lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		return std::lower_bound(haystack + (before_first + 1), haystack + after_last, needle);
	}

	template <typename Type>
	long upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		return std::upper_bound(haystack + (before_first + 1), haystack + after_last, needle);
	}
}