#pragma once
#include <stddef.h>

namespace InterlacedDoubleBinaryHeap
{
	template <typename Type>
	void build(Type *data, std::size_t first, std::size_t after_last)
	{
		// TODO: RESEARCH: keep heap structure on both sides.
		//     alternate on build-min-heap and build-max-heap calls (using the recursive variant, not iterative)
		//     when a element must be swapped, get the complementary_node using (approximately) `1 - n`
		//     if (complementary_node is meaningful i.e. it is not on the same side as gibberish values according to a partition)
		//         if (complementary_node invalidates it's parent)
		//             "sift-up"
		//         else if (complementary_node invalidates it's children)
		//             "sift-down"
	}
}