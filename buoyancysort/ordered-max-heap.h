#pragma once

#include <algorithm>
#include <vector>
#include "max-heap.h"

namespace OrderedMaxHeap
{
	void initialize(long before_first, long after_last, long tuple_size,
			std::vector<bool> &trusty_matrix,
			std::vector<char> &depth_matrix,
			std::vector<std::vector<long>> &dubious_max_nodes,
			std::vector<std::vector<long>> &next_dubious_max_nodes,
			long &max_left_line_of_implicit_trust)
	{
		max_left_line_of_implicit_trust = MaxHeap::parent(before_first + 1, after_last, tuple_size) - 1; // when on a line of trust, a node is still trusted
		// a heap on the bottom half (i.e. no children) is implicitly trusted

		long bitset_size = after_last - before_first + 1;
		long heap_depth;
		trusty_matrix.resize(bitset_size); // To index into this array, always subtract before_first (contains min and max definitions -- since bottom half rounded up is implicitly trusted).
		depth_matrix.resize(bitset_size); // NOTE: Theoretically, this *could* fail with certain char/long definitions (sort of undefined behavior)

		long array_index = 0;
		long memset_index_begin = 1;
		long memset_index_end = memset_index_begin + 1;

		// Because of the nature of next_power_of_k() (time complexity ~O(lg(lg(N))) I think),
		// I have to cache the powers_of_k that are used in dubious_min/max_nodes.
		while (memset_index_begin < bitset_size - 1)
		{
			long end = std::min(bitset_size - 1, memset_index_end);
			long offset = memset_index_begin;
			long length = end - memset_index_begin;
			memset(depth_matrix.data() + (before_first + 1) + offset, array_index, length);
			heap_depth = array_index;
			dubious_max_nodes.resize(heap_depth + 1);
			next_dubious_max_nodes.resize(heap_depth + 1);
			dubious_max_nodes[heap_depth].reserve(length);
			for (int forward_index = offset; forward_index < end; forward_index += 1)
			{
				if (after_last - forward_index >= max_left_line_of_implicit_trust)
				{
					break;
				}
				dubious_max_nodes[heap_depth].push_back(after_last - forward_index);
			}
			dubious_max_nodes[heap_depth].shrink_to_fit();
			array_index += 1;
			memset_index_begin *= tuple_size;
			memset_index_end *= tuple_size;
		}
	}
}