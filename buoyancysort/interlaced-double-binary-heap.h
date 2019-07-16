#pragma once

#include <stddef.h>
#include "heap.h"
#include "power-of-two.h"

namespace InterlacedDoubleBinaryHeap
{
	template <typename Type>
	void build(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t size = after_last - first;
		std::size_t build_index = size;
		if (size > 1)
		{
			std::size_t last = after_last - 1;
			if (size % 2 == 1)
			{
				std::size_t last = after_last - 1;
				std::size_t parent_of_first = Heap::parent(first, last);
				if (data[first] > data[parent_of_first])
				{
					std::swap(data[first], data[parent_of_first]);
				}
				std::size_t parent_of_last = Heap::parent(last, first);
				if (data[parent_of_last] > data[last])
				{
					std::swap(data[parent_of_last], data[last]);
				}
			}
			build_index -= 1;
			std::size_t lower_power = PowerOfTwo::lower_power_of_two(size);
			for (std::size_t heap_row_remainder = size; heap_row_remainder > lower_power; size -= 1)
			{

				build_index -= 2;
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