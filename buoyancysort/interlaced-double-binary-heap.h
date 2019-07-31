#pragma once

#include <algorithm>
#include <limits.h>
#include <set>
#include <stddef.h>
#include <string.h>
#include <vector>
#include "min-heap.h"
#include "max-heap.h"
#include "power-of-two.h"

namespace InterlacedDoubleBinaryHeap
{
	template <typename Type>
	void lazy_build(Type *data, long before_first, long after_last)
	{
		long max_heapify_from = MaxHeap::parent(before_first+1, after_last);
		long min_heapify_from = MinHeap::parent(after_last-1, before_first);
		while (max_heapify_from < after_last)
		{
			MaxHeap::heapify(data, before_first, after_last, max_heapify_from);
			MinHeap::heapify(data, before_first, after_last, min_heapify_from);
			max_heapify_from += 1;
			min_heapify_from -= 1;
		}
	}

	template <typename Type>
	void get_dubious_min_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_min_nodes, long dubious_node, long min_left_line_of_trust, long min_right_line_of_trust, long max_left_line_of_trust, long max_right_line_of_trust)
	{
		if (dubious_node < max_right_line_of_trust)
		{
			while (dubious_node < max_right_line_of_trust)
			{
				if (min_left_line_of_trust < dubious_node && dubious_node < min_right_line_of_trust)
				{
					dubious_min_nodes.insert(dubious_node);
				}
				dubious_node = MaxHeap::parent(dubious_node, after_last);
			}
			if (min_left_line_of_trust < dubious_node && dubious_node < min_right_line_of_trust)
			{
				dubious_min_nodes.insert(dubious_node);
			}
		}
	}

	template <typename Type>
	void get_dubious_max_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_max_nodes, long dubious_node, long min_left_line_of_trust, long min_right_line_of_trust, long max_left_line_of_trust, long max_right_line_of_trust)
	{
		if (dubious_node > min_left_line_of_trust)
		{
			while (dubious_node > min_left_line_of_trust)
			{
				if (max_left_line_of_trust < dubious_node && dubious_node < max_right_line_of_trust)
				{
					dubious_max_nodes.insert(dubious_node);
				}
				dubious_node = MinHeap::parent(dubious_node, before_first); // I feel like this is a logical fallacy (it's coupled to how the parenting system works). If I didn't use before_first and after_last this would fail hard.
			}
			if (max_left_line_of_trust < dubious_node && dubious_node < max_right_line_of_trust)
			{
				dubious_max_nodes.insert(dubious_node);
			}
		}
	}

	template <typename Type>
	void enhance_dubious_min_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_min_nodes)
	{
		std::set<long> all_dubious_nodes;
		for (long dubious_node : dubious_min_nodes)
		{
			while (dubious_node > before_first)
			{
				all_dubious_nodes.insert(dubious_node);
				dubious_node = MinHeap::parent(dubious_node, before_first);
			}
		}
		dubious_min_nodes = all_dubious_nodes;
	}

	template <typename Type>
	void enhance_dubious_max_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_max_nodes)
	{
		std::set<long> all_dubious_nodes;
		for (long dubious_node : dubious_max_nodes)
		{
			while (dubious_node < after_last)
			{
				all_dubious_nodes.insert(dubious_node);
				dubious_node = MaxHeap::parent(dubious_node, after_last);
			}
		}
		dubious_max_nodes = all_dubious_nodes;
	}

	void initialize(long before_first, long after_last,
			std::vector<bool> &trusty_matrix,
			std::vector<char> &depth_matrix,
			std::vector<std::vector<long>> &dubious_min_nodes,
			std::vector<std::vector<long>> &dubious_max_nodes,
			std::vector<std::vector<long>> &next_dubious_min_nodes,
			std::vector<std::vector<long>> &next_dubious_max_nodes)
	{
		long bitset_size = after_last - before_first + 1;
		long heap_depth;
		trusty_matrix.resize(bitset_size); // To index into this array, always subtract before_first (contains min and max definitions -- since bottom half rounded up is implicitly trusted).
		depth_matrix.resize(bitset_size); // NOTE: Theoretically, this *could* fail with certain char/long definitions (sort of undefined behavior)

		long array_index = 0;
		long memset_index_begin = 1;
		long memset_index_end = memset_index_begin + 1;

		// the future topple() function no longer has a sentinel node because of this choice =/
		trusty_matrix[0] = true;
		trusty_matrix[bitset_size-1] = true;
		if ((bitset_size % 2) == 1)
		{
			trusty_matrix[bitset_size/2] = true;
		}

		// Because of the nature of next_power_of_two() (time complexity ~O(lg(lg(N))) I think),
		// I have to cache the powers_of_two that are used in dubious_min/max_nodes.
		while (memset_index_begin < bitset_size-1)
		{
			long end = std::min(bitset_size - 1, memset_index_end);
			long offset = memset_index_begin;
			long length = end - memset_index_begin;
			memset(depth_matrix.data() + (before_first + 1) + offset, array_index, length);
			heap_depth = array_index;
			dubious_min_nodes.resize(heap_depth + 1);
			dubious_max_nodes.resize(heap_depth + 1);
			next_dubious_min_nodes.resize(heap_depth + 1);
			next_dubious_max_nodes.resize(heap_depth + 1);
			dubious_min_nodes[heap_depth].reserve(length);
			dubious_max_nodes[heap_depth].reserve(length);
			for (int forward_index = offset; forward_index < end; forward_index += 1)
			{
				dubious_min_nodes[heap_depth].push_back(before_first + forward_index);
				dubious_max_nodes[heap_depth].push_back(after_last - forward_index);
			}

			array_index += 1;
			memset_index_begin *= 2;
			memset_index_end *= 2;
		}

		std::cout << depth_matrix.size();
		std::cout << std::endl;
		std::cout << std::endl;
		for (bool trust : trusty_matrix)
		{
			std::cout << ((int)trust) << " ";
		}
		std::cout << std::endl;

		std::cout << std::endl;
		for (char parent : depth_matrix)
		{
			std::cout << ((int)parent) << " ";
		}
		std::cout << std::endl;

		std::cout << std::endl;
		for (std::vector<long> subarray : dubious_min_nodes)
		{
			for (long index : subarray)
			{
				std::cout << index << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (std::vector<long> subarray : dubious_max_nodes)
		{
			for (long index : subarray)
			{
				std::cout << index << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	bool cached_trust_min_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long min_right_line_of_implicit_trust)
	{
		if (node >= min_right_line_of_implicit_trust)
		{
			return true;
		}
		return trusty_matrix[node - before_first];
	}

	bool test_trust_min_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long min_right_line_of_implicit_trust)
	{
		long right_child = MinHeap::right_child(node, before_first);
		long left_child = right_child - 1;
		return cached_trust_min_node(before_first, after_last, trusty_matrix, left_child, min_right_line_of_implicit_trust) &&
				cached_trust_min_node(before_first, after_last, trusty_matrix, right_child, min_right_line_of_implicit_trust);
	}

	bool cached_trust_max_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long max_left_line_of_implicit_trust)
	{
		if (node <= max_left_line_of_implicit_trust)
		{
			return true;
		}
		return trusty_matrix[node - before_first];
	}

	bool test_trust_max_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long max_left_line_of_implicit_trust)
	{
		long left_child = MaxHeap::left_child(node, after_last);
		long right_child = left_child + 1;
		return cached_trust_min_node(before_first, after_last, trusty_matrix, left_child, max_left_line_of_implicit_trust) &&
			cached_trust_min_node(before_first, after_last, trusty_matrix, right_child, max_left_line_of_implicit_trust);
	}

	template <typename Type>
	void build(Type *data, long before_first, long after_last)
	{
		std::vector<bool> trusty_matrix;
		std::vector<char> depth_matrix;
		std::vector<std::vector<long>> dubious_min_nodes;
		std::vector<std::vector<long>> dubious_max_nodes;
		std::vector<std::vector<long>> next_dubious_min_nodes;
		std::vector<std::vector<long>> next_dubious_max_nodes;

		initialize(before_first, after_last,
				trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, next_dubious_min_nodes, next_dubious_max_nodes);

		long max_left_line_of_implicit_trust = MaxHeap::parent(before_first + 1, after_last) - 1; // when on a line of trust, a node is still trusted
		long min_right_line_of_implicit_trust = MinHeap::parent(after_last - 1, before_first) + 1; // a heap on the bottom half (i.e. no children) is implicitly trusted

		while (true)
		{
			bool change_attempted = false;
			for (long depth = dubious_min_nodes.size() - 1; depth >= 0; depth -= 1)
			{
				if (dubious_min_nodes[depth].size() > 0 || dubious_max_nodes[depth].size() > 0)
				{
					change_attempted = true;
					while (dubious_min_nodes[depth].size() < dubious_max_nodes[depth].size())
					{
						//MaxHeap::heapify(some iterator)
					}
					while (dubious_max_nodes[depth].size() < dubious_min_nodes[depth].size())
					{
						//MinHeap::heapify(some iterator)
					}
					while (dubious_min_nodes[depth].size() > 0)
					{
						//MinHeap::heapify(some iterator)
						//MaxHeap::heapify(some iterator)
					}
				}
			}
			if (!change_attempted)
			{
				break;
			}
		}
		/*
		while (max_right_line_of_explicit_trust < after_last)
		{
			dubious_node = MaxHeap::heapify(data, before_first, after_last, max_right_line_of_explicit_trust);
			max_right_line_of_explicit_trust += 1;
			get_dubious_min_heap_elements(data, before_first, after_last, dubious_min_nodes, dubious_node, min_left_line_of_explicit_trust, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust, max_right_line_of_explicit_trust);

			dubious_node = MinHeap::heapify(data, before_first, after_last, min_left_line_of_explicit_trust);
			min_left_line_of_explicit_trust -= 1;
			get_dubious_max_heap_elements(data, before_first, after_last, dubious_max_nodes, dubious_node, min_left_line_of_explicit_trust, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust, max_right_line_of_explicit_trust);
		}
		*/
		std::cout << std::endl;
		for (std::vector<long> subarray : dubious_min_nodes)
		{
			for (long index : subarray)
			{
				std::cout << index << " ";
			}
		}
		for (std::vector<long> subarray : dubious_max_nodes)
		{
			for (long index : subarray)
			{
				std::cout << index << " ";
			}
		}
		std::cout << std::endl;
	}

	// use min_heapify() and max_heapify() in an alternating fasion (as subroutines).
	// when the return isn't the index, swaps were made and you are given a descendent child.
	// Find the list of decendents all the way up until the ancestor.
	// Add that list of decendents to a std::ordered_set<std::size_t> of dubious_nodes
	// At least when those nodes are past the "lines of trust".
	// TODO: RESEARCH: how do "lines of trust" work? When are they invalidated? When are they still valid/"trusty"?
	// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
}