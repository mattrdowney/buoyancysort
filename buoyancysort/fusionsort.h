#pragma once

#include <algorithm>
#include <stddef.h>

namespace Fusionsort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last)
	{
		// E.g.:
		// 0/1-indexed min "ordered heap": 1 1 2 4 8 16
		// Build a heap covering all 32 elements
		// Build a heap covering the last 16 elements and stabilize the 16 -> 32 -> 16 (-> 32 -> 16)* heaps 
		// Build a heap covering the last 8 elements and stabilize the 8 -> 16 -> 32 -> 16 -> 8 ( -> 16 -> 32 -> 16 -> 8)* heaps
		// Build a heap covering the last 4 elements and stabilize the 4 (-> 8 -> 16 -> 32 -> 16 -> 8 -> 4)+ heaps
		// Build a heap covering the last 2 elements and stabilize the 2 (-> 4 -> 8 -> 16 -> 32 -> 16 -> 8 -> 4 -> 2)+ heaps
		// Now that all of the heaps are stable:
		//     there's a possibility you have gotten elements sorted within O(lg(lg(N))) places of their final position in O(lg(lg(N))) time
		//     you have to do an adaptive mergesort (e.g. Timsort) in O(lg(lg(N))) time to get elements to their final position.
	}
}