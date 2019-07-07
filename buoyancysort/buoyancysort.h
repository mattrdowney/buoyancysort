#pragma once

#include <algorithm>
#include <math.h>
#include <stddef.h>
#include "interlaced-double-binary-heap.h"

namespace Buoyancysort
{
	template <typename Type>
	void sort(Type *data, std::size_t first, std::size_t after_last, std::size_t chunk_size)
	{
		InterlacedDoubleBinaryHeap::build(data, first, after_last);
		if (after_last - first + 1 >= chunk_size)
		{
			//hierarchysort
			return;
		}
		// if (size is less than e.g. O(N/lglgN) where N is the size of the original array)
		//     Build-Interlaced-Double-Binary-Heap(1...n)
		//     Timsort(1...n)
		//     return
		// Build-Interlaced-Double-Binary-Heap(1...n) // preferably keeping heap structure on both sides
		// For forwards and backwards directions:
		//     "Insertion Sort" while O(a_constant) to sort a region of size "first_known_inversion" 
		//     Skip to 2*"first_known_inversion"
		//     Bubble-Sort back greatest/least element to position "first_known_inversion"
		//     "Insertion-Sort" back that greatest/least element into sorted subarray
		//     After this point, all values preceding or equal to that element are sorted and can be ignored
		// "Insertion-Sort" on values in the range 4/16n : 1/16n : 12/16n
		// pivot = Hoare-Partition(1...n) with the "Insertion-Sort"ed value at ~8/16n (from prior step) as the partition line
		// if pivot is bad (outside of inter-quartile range i.e. middle 50%) you need to fallback to statistical sampling then fallback to median of medians.
		// (optional) for left and right partition:
		//    While (left > e.g. O(nlglgn)) // ditto for right tail-end
		//        "Insertion-Sort" on values in the range 4/16n : 1/16n : 12/16n
		//        pivot = Hoare-Partition(1...n) with the "Insertion-Sort"ed value at ~8/16n (from prior step) as the partition line
		//        left = pivot
		// Buoyancysort(first_unsorted_left...pivot - 1)
		// Buoyancysort(pivot + 1...first_unsorted_right)

		// define Build-Interlaced-Double-Binary-Heap()
		//     alternate on build-min-heap and build-max-heap calls (using the recursive variant, not iterative)
		//     when a element must be swapped, get the complementary_node using (approximately) `1 - n`
		//     if (complementary_node is meaningful i.e. it is not on the same side as gibberish values according to a partition)
		//         if (complementary_node invalidates it's parent)
		//             "sift-up"
		//         else if (complementary_node invalidates it's children)
		//             "sift-down"
	}

	template <typename Type>
	void sort(Type *data, std::size_t count)
	{
		const unsigned long long minimum_chunk_size = 256;
		unsigned long long asymptotic_chunk_size = (unsigned long long)(((double)count) / log(log((double)count)));
		unsigned long long unclamped_chunk_size = std::max(minimum_chunk_size, asymptotic_chunk_size);
		std::size_t chunk_size = (std::size_t)std::min((unsigned long long)count, unclamped_chunk_size);
		sort(data, 0, count, chunk_size); // NOTE: count = after_last
	}
}