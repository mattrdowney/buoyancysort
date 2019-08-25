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
		long memset_index_begin = 1; // FIXME: This isn't quite right...
		long memset_index_size = 1;
		long memset_index_end = 2;

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
			memset_index_begin = memset_index_end;
			memset_index_size *= tuple_size;
			memset_index_end = memset_index_begin + memset_index_size;
		}
	}

	template <typename Type>
	void sort_min_siblings(Type *data, long before_first, long after_last, long tuple_size, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, long min_right_line_of_implicit_trust)
	{
		long left_child = MinHeap::left_child(root, before_first, tuple_size);
		long right_child = MinHeap::right_child(root, before_first, tuple_size);
		// Start - Selection Sort
		for (long child = left_child; child < right_child; child += 1)
		{
			long minimum = child;
			for (long min_selector = child + 1; min_selector <= right_child; min_selector += 1)
			{
				minimum = (data[minimum] < data[min_selector] ? minimum : min_selector);
			}
			if (minimum != child)
			{
				if (minimum < min_right_line_of_implicit_trust && trusty_matrix[minimum] == true)
				{
					// TODO: VERIFY: something feels off
					// You can no longer trust the node whose value increased (since it may no longer be a heap).
					trusty_matrix[minimum] = false;
					next_dubious_min_nodes[depth_matrix[minimum - before_first]].push_back(minimum);
				}
				std::swap(data[child], data[minimum]);
			}
		}
		// End - Selection Sort
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
						sort_min_siblings(data, before_first, after_last, tuple_size, dubious_min_siblings[depth][min_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, min_right_line_of_implicit_trust);
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