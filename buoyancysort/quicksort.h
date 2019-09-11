#pragma once

// I sort of want to implement a hybrid quicksort that doesn't use insertion sort (but instead ShellSort).
// The basic idea: sort objects up until the point they fit into the cache well (by stopping quicksort when subarrays are of size k).
// After that, use Tokuda's ShellSort in a broadphase pass.
// If quicksort always went left to right you could shellsort right to left to have fewer cache misses (but, in practice, quicksort is implemented to recur on the smaller array first).

// If I'm right about there being faster sorts, I think this is a candidate that could perform better than O(n*lg(n)) because HoarePartition would push elements towards their final positions a little bit faster -- middle elements would typically be in the middle, large elements would typically be on the right, small elements would typically be left.

#include <math.h>
#include <utility>
#include "median-of-medians.h"
#include "shell-sort.h"
namespace Quicksort
{
	// okay, we might've did it: on reverse sorted data I got 
	template <typename Type>
	void not_quicksort(Type *data, long before_first, long after_last, long cutoff, bool even)
	{
		if (after_last - (before_first + 1) > cutoff)
		{
			long pivot;
			if (even)
			{
				pivot = MedianOfMedians::medianplex3<Type, 8>(data, before_first, after_last);
			}
			else
			{
				pivot = MedianOfMedians::medianplex5<Type, 13>(data, before_first, after_last);
			}
			std::pair<long, long> partitions = StablePartition::two_way(data, before_first, after_last, pivot);
			not_quicksort(data, before_first, partitions.first, cutoff, !even);
			not_quicksort(data, partitions.second, after_last, cutoff, !even);
		}
	}

	template <typename Type>
	void not_quicksort(Type *data, long before_first, long after_last)
	{
		long size = (after_last - (before_first + 1));
		long cutoff = size / log2(log2(size)) / 2;
		not_quicksort(data, before_first, after_last, cutoff, true);
		cutoff = size / pow(log2(size), 1.5);
		ShellSort::sort(data, before_first, after_last, ShellSort::ciura_extended_gap_sequence_attempt1, cutoff);
	}
}