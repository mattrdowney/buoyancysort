#pragma once
#include <stddef.h>
#include "heap.h"

namespace InterlacedDoubleBinaryHeap
{
	template <typename Type>
	void build(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t size = after_last - first;
		if (size > 0)
		{
			std::size_t last = after_last - 1;
			if (size % 2 == 1)
			{
				std::size_t last_parent
			}
		}
	}

	// TODO: RESEARCH: keep heap structure on both sides.
	//     alternate on build-min-heap and build-max-heap calls (using the recursive variant, not iterative)
	//     when a element must be swapped, get the complementary_node using (approximately) `1 - n`
	//     if (complementary_node is meaningful i.e. it is not on the same side as gibberish values according to a partition)
	//         if (complementary_node invalidates it's parent)
	//             "sift-up"
	//         else if (complementary_node invalidates it's children)
	//             "sift-down"
}