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
#include "stable-partition.h"
#include "unstable-partition.h"

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
			std::pair<long, long> partitions;
			if (even)
			{
				// I think I finally figured out what I'm trying to do here (e.g. when I say sorting via statistics)
				// I want to augment the data so that it's an array of std::pair<Type, long> where the second index is the number of times the value was in the lower (or correct?) partition -- it's sort of an abstract idea right now.
				// At the end of the sort, the minimum element will be in the lower partition O(lgn) times and the maximum will have been in the lower partition 0 times (if you actually ran to completion).
				// Because lower and upper partition are sort of ill-defined for e.g. Hoare this makes it a little interesting.
				// You basically want to do a left/right partition then do a stable counting sort to order the elements via statistics.
				// I think you do want to use Hoare, (the counting sort is part where stability is important), so you probably need two counters: one for the number of times it was swapped up, another for the times it was swapped down. I think this counter should actually use the number of positions it was moved (being swapped up 1 position and down 100 positions don't cancel out) and you need some way of disregarding information that's too old. This is probably Velocitysort. I think you basically store one integer for the number of positions moved up or down with the Hoare partition. You then do bin-sort on each of the velocities on each iteration. I'd have to think about this more, but you would want to ignore the number of positions moved to an extent and focus more on the number of runs/streaks you've moved in the same direction. So an element that has a streak of 0 (pivot) is in the middle, a streak of +1 means it's moving rightward slowly, a streak of -10 means it is a serious candidate for the minimum element in the sequence. This is super abstract, because I know a lot of these ideas wouldn't actually work, but I think I'm on to something.
				partitions = UnstablePartition::hoare(data, before_first, after_last, pivot);
			}
			else
			{
				partitions = StablePartition::two_way(data, before_first, after_last, pivot);
			}
			not_quicksort(data, before_first, partitions.first, cutoff, !even);
			not_quicksort(data, partitions.second, after_last, cutoff, !even);
		}
		else
		{
			ShellSort::sort(data, before_first, after_last, ShellSort::ciura_extended_gap_sequence_attempt1);
		}
	}

	template <typename Type>
	void not_quicksort(Type *data, long before_first, long after_last)
	{
		long size = (after_last - (before_first + 1));
		long cutoff = size / log2(log2(size)) / 2;
		not_quicksort(data, before_first, after_last, cutoff, true);
	}
}