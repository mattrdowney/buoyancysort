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

// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
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

	void initialize(long before_first, long after_last,
			std::vector<bool> &trusty_matrix,
			std::vector<char> &depth_matrix,
			std::vector<std::vector<long>> &dubious_min_nodes,
			std::vector<std::vector<long>> &dubious_max_nodes,
			std::vector<std::vector<long>> &next_dubious_min_nodes,
			std::vector<std::vector<long>> &next_dubious_max_nodes,
			long &min_right_line_of_implicit_trust,
			long &max_left_line_of_implicit_trust)
	{
		max_left_line_of_implicit_trust = MaxHeap::parent(before_first + 1, after_last) - 1; // when on a line of trust, a node is still trusted
		min_right_line_of_implicit_trust = MinHeap::parent(after_last - 1, before_first) + 1; // a heap on the bottom half (i.e. no children) is implicitly trusted

		long bitset_size = after_last - before_first + 1;
		long heap_depth;
		trusty_matrix.resize(bitset_size); // To index into this array, always subtract before_first (contains min and max definitions -- since bottom half rounded up is implicitly trusted).
		depth_matrix.resize(bitset_size); // NOTE: Theoretically, this *could* fail with certain char/long definitions (sort of undefined behavior)

		long array_index = 0;
		long memset_index_begin = 1;
		long memset_index_end = memset_index_begin + 1;

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
				if (before_first + forward_index >= min_right_line_of_implicit_trust)
				{
					break;
				}
				dubious_min_nodes[heap_depth].push_back(before_first + forward_index);
				dubious_max_nodes[heap_depth].push_back(after_last - forward_index);
			}
			dubious_min_nodes[heap_depth].shrink_to_fit();
			dubious_max_nodes[heap_depth].shrink_to_fit();
			array_index += 1;
			memset_index_begin *= 2;
			memset_index_end *= 2;
		}
	}

	bool cached_trust_min_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long min_right_line_of_implicit_trust)
	{
		if (node >= min_right_line_of_implicit_trust)
		{
			return true;
		}
		return trusty_matrix[node - before_first];
	}

	void set_trust_min_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long min_right_line_of_implicit_trust, bool state)
	{
		if (node >= min_right_line_of_implicit_trust)
		{
			if (state == false)
			{
				std::cout << std::endl << "Critical error: set_trust_min_node" << std::endl;
			}
			return;
		}
		trusty_matrix[node - before_first] = state;
	}

	bool test_trust_min_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long min_right_line_of_implicit_trust)
	{
		long right_child = MinHeap::right_child(node, before_first);
		long left_child = right_child - 1;
		// FIXME: make this actually test if the parent is smaller
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

	void set_trust_max_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long max_left_line_of_implicit_trust, bool state)
	{
		if (node <= max_left_line_of_implicit_trust)
		{
			if (state == false)
			{
				std::cout << std::endl << "Critical error: set_trust_max_node" << std::endl;
			}
			return;
		}
		trusty_matrix[node - before_first] = state;
	}

	bool test_trust_max_node(long before_first, long after_last, std::vector<bool> &trusty_matrix, long node, long max_left_line_of_implicit_trust)
	{
		long left_child = MaxHeap::left_child(node, after_last);
		long right_child = left_child + 1;
		// FIXME: make this actually test if the parent is larger
		return cached_trust_max_node(before_first, after_last, trusty_matrix, left_child, max_left_line_of_implicit_trust) &&
			cached_trust_max_node(before_first, after_last, trusty_matrix, right_child, max_left_line_of_implicit_trust);
	}

	template <typename Type>
	void min_heapify(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		long dubious_node = MinHeap::heapify(data, before_first, after_last, root);
		if (test_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust))
		{
			set_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust, true);
		}
		else
		{
			next_dubious_min_nodes[depth_matrix[root - before_first]].push_back(root);
		}
		while (dubious_node > root)
		{
			if (cached_trust_max_node(before_first, after_last, trusty_matrix, dubious_node, max_left_line_of_implicit_trust))
			{
				long topple_child_cursor = dubious_node;
				long topple_parent_cursor = MaxHeap::parent(topple_child_cursor, after_last);
				if (data[topple_child_cursor] > data[topple_parent_cursor])
				{
					while (cached_trust_max_node(before_first, after_last, trusty_matrix, topple_parent_cursor, max_left_line_of_implicit_trust))
					{
						next_dubious_max_nodes[depth_matrix[after_last - topple_parent_cursor]].push_back(topple_parent_cursor);
						set_trust_max_node(before_first, after_last, trusty_matrix, topple_parent_cursor, max_left_line_of_implicit_trust, false);
						topple_child_cursor = topple_parent_cursor;
						topple_parent_cursor = MaxHeap::parent(topple_parent_cursor, after_last);
					}
				}
			}
			dubious_node = MinHeap::parent(dubious_node, before_first);
		}
	}

	template <typename Type>
	void max_heapify(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		long dubious_node = MaxHeap::heapify(data, before_first, after_last, root);
		if (test_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust))
		{
			set_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust, true);
		}
		else
		{
			next_dubious_max_nodes[depth_matrix[after_last - root]].push_back(root);
		}
		while (dubious_node < root)
		{
			if (cached_trust_min_node(before_first, after_last, trusty_matrix, dubious_node, min_right_line_of_implicit_trust))
			{
				long topple_child_cursor = dubious_node;
				long topple_parent_cursor = MinHeap::parent(topple_child_cursor, before_first);
				if (data[topple_parent_cursor] > data[topple_child_cursor])
				{
					while (cached_trust_min_node(before_first, after_last, trusty_matrix, topple_parent_cursor, min_right_line_of_implicit_trust))
					{
						next_dubious_min_nodes[depth_matrix[topple_parent_cursor - before_first]].push_back(topple_parent_cursor);
						set_trust_min_node(before_first, after_last, trusty_matrix, topple_parent_cursor, min_right_line_of_implicit_trust, false);
						topple_child_cursor = topple_parent_cursor;
						topple_parent_cursor = MinHeap::parent(topple_parent_cursor, before_first);
					}
				}
			}
			dubious_node = MaxHeap::parent(dubious_node, after_last);
		}
	}

	/// STATUS: believed to be final. No changes pending.
	template <typename Type>
	void build(Type *data, long before_first, long after_last,
		std::vector<bool> &trusty_matrix,
		std::vector<char> &depth_matrix,
		std::vector<std::vector<long>> &dubious_min_nodes,
		std::vector<std::vector<long>> &dubious_max_nodes,
		std::vector<std::vector<long>> &next_dubious_min_nodes,
		std::vector<std::vector<long>> &next_dubious_max_nodes,
		long &min_right_line_of_implicit_trust,
		long &max_left_line_of_implicit_trust)
	{
		while (true)
		{
			bool change_attempted = false;
			for (long depth = dubious_min_nodes.size() - 1; depth >= 0; depth -= 1)
			{
				if (dubious_min_nodes[depth].size() > 0 || dubious_max_nodes[depth].size() > 0)
				{
					change_attempted = true;
					int min_iterator = dubious_min_nodes[depth].size();
					int max_iterator = dubious_max_nodes[depth].size();
					while (min_iterator < max_iterator)
					{
						max_iterator -= 1;
						max_heapify(data, before_first, after_last, dubious_max_nodes[depth][max_iterator], trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					while (min_iterator > max_iterator)
					{
						min_iterator -= 1;
						min_heapify(data, before_first, after_last, dubious_min_nodes[depth][min_iterator], trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					while (min_iterator > 0)
					{
						min_iterator -= 1;
						max_iterator -= 1;
						min_heapify(data, before_first, after_last, dubious_min_nodes[depth][min_iterator], trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
						max_heapify(data, before_first, after_last, dubious_max_nodes[depth][max_iterator], trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					dubious_min_nodes[depth].clear();
					dubious_max_nodes[depth].clear();
				}
			}
			if (!change_attempted)
			{
				break;
			}
			dubious_min_nodes.swap(next_dubious_min_nodes);
			dubious_max_nodes.swap(next_dubious_max_nodes);
		}
	}

	/// STATUS: believed to be final. No changes pending.
	template <typename Type>
	void build(Type *data, long before_first, long after_last)
	{
		std::vector<bool> trusty_matrix;
		std::vector<char> depth_matrix;
		std::vector<std::vector<long>> dubious_min_nodes;
		std::vector<std::vector<long>> dubious_max_nodes;
		std::vector<std::vector<long>> next_dubious_min_nodes;
		std::vector<std::vector<long>> next_dubious_max_nodes;
		long min_right_line_of_implicit_trust = 0; // a heap's bottom half (i.e. the elements with no children) is implicitly trusted
		long max_left_line_of_implicit_trust = 0; // when on a line of trust (equal to), a node is still trusted

		initialize(before_first, after_last,
				trusty_matrix, depth_matrix,
				dubious_min_nodes, dubious_max_nodes,
				next_dubious_min_nodes, next_dubious_max_nodes,
				min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);

		build(data, before_first, after_last,
				trusty_matrix, depth_matrix,
				dubious_min_nodes, dubious_max_nodes,
				next_dubious_min_nodes, next_dubious_max_nodes,
				min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
	}
}