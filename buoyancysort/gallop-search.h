#pragma once

#include <algorithm>
#include "binary-search.h"

// This code is based on https://github.com/tvanslyke/timsort-cpp/blob/master/include/tim/utils.h
// The algorithm is generally called https://en.wikipedia.org/wiki/Exponential_search (although that name makes more sense in the context of infinite arrays).
namespace GallopSearch
{
	template <typename Type>
	long forward_lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		long gallop_cursor = 1;
		long size = (after_last - (before_first + 1));
		while (gallop_cursor <= size && haystack[before_first + gallop_cursor] < needle)
		{
			gallop_cursor *= 2;
		}

		if (gallop_cursor > size)
		{
			gallop_cursor = size;
		}

		return BinarySearch::lower_bound(haystack, before_first + gallop_cursor/2, before_first + gallop_cursor + 1, needle);
	}

	template <typename Type>
	long forward_upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		long gallop_cursor = 1;
		long size = (after_last - (before_first + 1));
		while (gallop_cursor <= size && haystack[before_first + gallop_cursor] <= needle)
		{
			gallop_cursor *= 2;
		}

		if (gallop_cursor > size)
		{
			gallop_cursor = size;
		}

		return BinarySearch::upper_bound(haystack, before_first + gallop_cursor/2, before_first + gallop_cursor + 1, needle);
	}

	template <typename Type>
	long reverse_lower_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		long gallop_cursor = 1;
		long size = (after_last - (before_first + 1));
		while (gallop_cursor <= size && haystack[after_last - gallop_cursor] >= needle)
		{
			gallop_cursor *= 2;
		}

		if (gallop_cursor > size)
		{
			gallop_cursor = size;
		}

		return BinarySearch::lower_bound(haystack, after_last - gallop_cursor - 1, after_last - gallop_cursor/2, needle);
	}

	template <typename Type>
	long reverse_upper_bound(Type *haystack, long before_first, long after_last, Type needle)
	{
		long gallop_cursor = 1;
		long size = (after_last - (before_first + 1));
		while (gallop_cursor <= size && haystack[after_last - gallop_cursor] > needle)
		{
			gallop_cursor *= 2;
		}

		if (gallop_cursor > size)
		{
			gallop_cursor = size;
		}

		return BinarySearch::upper_bound(haystack, after_last - gallop_cursor - 1, after_last - gallop_cursor / 2, needle);
	}
}