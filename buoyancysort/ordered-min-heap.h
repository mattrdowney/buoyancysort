#pragma once

#include <algorithm>
#include <vector>
#include "min-heap.h"

namespace OrderedMinHeap
{
	void initialize(long before_first, long after_last, long tuple_size,
			std::vector<bool> &trusty_matrix,
			std::vector<char> &depth_matrix,
			std::vector<std::vector<long>> &dubious_min_nodes,
			std::vector<std::vector<long>> &next_dubious_min_nodes,
			long &min_right_line_of_implicit_trust)
	{
		// when on a line of trust, a node is still trusted
		min_right_line_of_implicit_trust = MinHeap::parent(after_last - 1, before_first, tuple_size) + 1; // a heap on the bottom half (i.e. no children) is implicitly trusted

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
			dubious_min_nodes.resize(heap_depth + 1);
			next_dubious_min_nodes.resize(heap_depth + 1);
			dubious_min_nodes[heap_depth].reserve(length);
			for (int forward_index = offset; forward_index < end; forward_index += 1)
			{
				if (before_first + forward_index >= min_right_line_of_implicit_trust)
				{
					break;
				}
				dubious_min_nodes[heap_depth].push_back(before_first + forward_index);
			}
			dubious_min_nodes[heap_depth].shrink_to_fit();
			array_index += 1;
			memset_index_begin *= tuple_size;
			memset_index_end *= tuple_size;
		}
	}

	template <typename Type>
	void sort_min_siblings(Type *data, long before_first, long after_last, long tuple_size, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		long left_child = MinHeap::left_child(root, before_first, tuple_size);
		long right_child = MinHeap::right_child(root, before_first, tuple_size);
		if (right_child < after_last && data[left_child] > data[right_child])
		{
			// It's tempting to use cycle sort here, but I will not (at least for now)
			// Interestingly, I think the best sort here is Selection Sort.
			// If you find an item that isn't already in its correct position, then the swapping item's order must have increased.
			// Additionally, because sorted lists are removed (the O(k) best case is less important)
			// Selection Sort has a time complexity very similar to Insertion Sort in the worst case,
			// it avoids some branch prediction problems,
			// it reduces the number of swaps,
			// and we are dealing with a small number of elements (O(k) e.g. 5 elements).
			std::swap(data[left_child], data[right_child]);
			verify_min_stability(data, before_first, after_last, right_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
			verify_max_stability(data, before_first, after_last, left_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
		}
	}

	template <typename Type>
	void build(Type *data, long before_first, long after_last, long tuple_size)
	{
		// TODO: Have I created a memory leak anywhere? I forgot I should be checking for that.
		std::vector<bool> trusty_matrix;
		std::vector<char> depth_matrix;
		std::vector<std::vector<long>> dubious_min_nodes;
		std::vector<std::vector<long>> next_dubious_min_nodes;
		long min_right_line_of_implicit_trust = 0; // a heap's bottom half (i.e. the elements with no children) is implicitly trusted
		// when on a line of trust (equal to), a node is still trusted

		initialize(before_first, after_last, tuple_size,
			trusty_matrix, depth_matrix,
			dubious_min_nodes,
			next_dubious_min_nodes,
			min_right_line_of_implicit_trust);

		std::vector<std::vector<long>> dubious_min_siblings(dubious_min_nodes); // Attempting deep copy, which is important because this...
		// ... is a vector<vector>(the inner vectors are .clear()'ed)

		while (true)
		{
			// Start - build heap
			for (long depth = dubious_min_nodes.size() - 1; depth >= 0; depth -= 1)
			{
				if (dubious_min_nodes[depth].size() > 0)
				{
					int min_iterator = dubious_min_nodes[depth].size();
					while (min_iterator > 0)
					{
						min_iterator -= 1;
						MinHeap::heapify(data, before_first, after_last, dubious_min_nodes[depth][min_iterator], tuple_size);
					}
					dubious_min_nodes[depth].clear();
				}
			}
			// End - build heap

			// Start - sort siblings
			bool change_attempted = false;
			for (long depth = dubious_min_siblings.size() - 1; depth >= 0; depth -= 1)
			{
				if (dubious_min_siblings[depth].size() > 0)
				{
					change_attempted = true;
					int min_iterator = dubious_min_siblings[depth].size();
					while (min_iterator > 0)
					{
						min_iterator -= 1;
						sort_min_siblings(data, before_first, after_last, dubious_min_siblings[depth][min_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					dubious_min_siblings[depth].clear();
				}
			}
			// End - sort siblings

			// copy dubious_min/max_nodes into dubious_min/max_siblings (attempt to avoid a memory leak)
			for (std::size_t depth = 0; depth < dubious_min_siblings.size(); depth += 1)
			{
				for (std::size_t min_iterator = 0; min_iterator < dubious_min_nodes[depth].size(); min_iterator += 1)
				{
					dubious_min_siblings[depth].push_back(dubious_min_nodes[depth][min_iterator]);
				}
			}

			if (!change_attempted)
			{
				break;
			}
		}
	}
}