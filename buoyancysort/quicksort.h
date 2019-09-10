#pragma once

// I sort of want to implement a hybrid quicksort that doesn't use insertion sort (but instead ShellSort).
// The basic idea: sort objects up until the point they fit into the cache well (by stopping quicksort when subarrays are of size k).
// After that, use Tokuda's ShellSort in a broadphase pass.
// If quicksort always went left to right you could shellsort right to left to have fewer cache misses (but, in practice, quicksort is implemented to recur on the smaller array first).

// If I'm right about there being faster sorts, I think this is a candidate that could perform better than O(n*lg(n)) because HoarePartition would push elements towards their final positions a little bit faster -- middle elements would typically be in the middle, large elements would typically be on the right, small elements would typically be left.

namespace Quicksort
{
	template <typename Type>
	void hybrid_sort(Type *data, long before_first, long after_last)
	{
	}
}