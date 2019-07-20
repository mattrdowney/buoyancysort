#pragma once

#include <stddef.h>
#include "heap.h"
#include "power-of-two.h"

namespace InterlacedDoubleBinaryHeap
{
	template <typename Type>
	void build(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t max_heapify_from = Heap::parent(first, after_last - 1);
		std::size_t min_heapify_from = Heap::parent(after_last - 1, first);
		while (max_heapify_from < after_last - 1)
		{
			max_heapify(data, first, after_last, max_heapify_from);
			min_heapify(data, first, after_last, min_heapify_from);
			max_heapify_from += 1;
			min_heapify_from -= 1;
		}
	}

	// use min_heapify() and max_heapify() in an alternating fasion (as subroutines).
	// when the return isn't the index, swaps were made and you are given a descendent child.
	// Find the list of decendents all the way up until the ancestor.
	// Add that list of decendents to a std::ordered_set<std::size_t> of dubious_nodes
	// At least when those nodes are past the "lines of trust".
	// TODO: RESEARCH: how do "lines of trust" work? When are they invalidated? When are they still valid/"trusty"?
	// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
}