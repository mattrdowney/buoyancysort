#pragma once

#include <vector>
#include "interlaced-double-binary-heap.h"
#include "min-heap.h"
#include "max-heap.h"

// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
namespace InterlacedEntangledDoubleBinaryHeap // even if this doesn't work, it's still interesting to develop
{
	template <typename Type>
	void verify_min_stability(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		if (InterlacedDoubleBinaryHeap::cached_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust))
		{
			long left_child = MinHeap::left_child(root, before_first);
			long right_child = left_child + 1;
			// Avoid a subtle bug when no children exist (or only one)
			bool invalid_left = (left_child < after_last && data[root] > data[left_child]);
			bool invalid_right = (right_child < after_last && data[root] > data[right_child]);
			if (invalid_left || invalid_right)
			{
				// topple the current root and all parent nodes
				long topple_cursor = root;
				while (InterlacedDoubleBinaryHeap::cached_trust_min_node(before_first, after_last, trusty_matrix, topple_cursor, min_right_line_of_implicit_trust))
				{
					next_dubious_min_nodes[depth_matrix[topple_cursor - before_first]].push_back(topple_cursor);
					InterlacedDoubleBinaryHeap::set_trust_min_node(before_first, after_last, trusty_matrix, topple_cursor, min_right_line_of_implicit_trust, false);
					topple_cursor = MinHeap::parent(topple_cursor, before_first);
				}
			}
		}
	}

	template <typename Type>
	void verify_max_stability(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		if (InterlacedDoubleBinaryHeap::cached_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust))
		{
			long right_child = MaxHeap::right_child(root, after_last);
			long left_child = right_child - 1;
			// Avoid a subtle bug when no children exist (or only one)
			bool invalid_left = (before_first < left_child && data[left_child] > data[root]);
			bool invalid_right = (before_first < right_child && data[right_child] > data[root]);
			if (invalid_left || invalid_right)
			{
				// topple the current root and all parent nodes
				long topple_cursor = root;
				while (InterlacedDoubleBinaryHeap::cached_trust_max_node(before_first, after_last, trusty_matrix, topple_cursor, max_left_line_of_implicit_trust))
				{
					next_dubious_min_nodes[depth_matrix[after_last - topple_cursor]].push_back(topple_cursor);
					InterlacedDoubleBinaryHeap::set_trust_max_node(before_first, after_last, trusty_matrix, topple_cursor, max_left_line_of_implicit_trust, false);
					topple_cursor = MaxHeap::parent(topple_cursor, after_last);
				}
			}
		}
	}

	template <typename Type>
	void sort_min_siblings(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		long left_child = MinHeap::left_child(root, before_first);
		long right_child = left_child + 1;
		if (right_child < after_last && data[left_child] > data[right_child])
		{
			std::swap(data[left_child], data[right_child]);
			verify_min_stability(data, before_first, after_last, right_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
			if (right_child != after_last - 1)
			{
				long max_parent_of_left_child = MaxHeap::parent(left_child, after_last);
				long max_parent_of_right_child = MaxHeap::parent(right_child, after_last);
				verify_max_stability(data, before_first, after_last, max_parent_of_left_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
				if (max_parent_of_left_child != max_parent_of_right_child)
				{
					verify_max_stability(data, before_first, after_last, max_parent_of_right_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
				}
			}
			else // verify true root (i.e. after_last - 1)
			{
				verify_max_stability(data, before_first, after_last, after_last-1, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
			}
		}
	}

	template <typename Type>
	void sort_max_siblings(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		long right_child = MaxHeap::right_child(root, after_last);
		long left_child = right_child - 1;
		if (before_first < left_child && data[left_child] > data[right_child])
		{
			std::swap(data[left_child], data[right_child]);
			verify_max_stability(data, before_first, after_last, left_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
			if (left_child != before_first + 1)
			{
				long min_parent_of_left_child = MinHeap::parent(left_child, before_first);
				long min_parent_of_right_child = MinHeap::parent(right_child, before_first);
				verify_min_stability(data, before_first, after_last, min_parent_of_left_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
				if (min_parent_of_left_child != min_parent_of_right_child)
				{
					verify_min_stability(data, before_first, after_last, min_parent_of_right_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
				}
			}
			else // verify true root (i.e. before_first + 1)
			{
				verify_min_stability(data, before_first, after_last, before_first + 1, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
			}
		}
	}

	/// STATUS: 100% believed to be final and not a copy-paste error. -9999 changes pending.
	template <typename Type>
	void build(Type *data, long before_first, long after_last)
	{
		// TODO: Have I created a memory leak anywhere? I forgot I should be checking for that.
		std::vector<bool> trusty_matrix;
		std::vector<char> depth_matrix;
		std::vector<std::vector<long>> dubious_min_nodes;
		std::vector<std::vector<long>> dubious_max_nodes;
		std::vector<std::vector<long>> next_dubious_min_nodes;
		std::vector<std::vector<long>> next_dubious_max_nodes;
		long min_right_line_of_implicit_trust = 0; // a heap's bottom half (i.e. the elements with no children) is implicitly trusted
		long max_left_line_of_implicit_trust = 0; // when on a line of trust (equal to), a node is still trusted

		InterlacedDoubleBinaryHeap::initialize(before_first, after_last,
				trusty_matrix, depth_matrix,
				dubious_min_nodes, dubious_max_nodes,
				next_dubious_min_nodes, next_dubious_max_nodes,
				min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);

		std::vector<std::vector<long>> dubious_min_siblings(dubious_min_nodes); // Attempting deep copy, which is important because this...
		std::vector<std::vector<long>> dubious_max_siblings(dubious_max_nodes); // ... is a vector<vector>(the inner vectors are .clear()'ed)

		// TODO: remove the one node without a sibling on each side (if it exists)
		// this is a cheap O(1) -- VERIFY: would this actually mean less work in sort()? You probably can still re-add the node later (I should probably play it safe)

		while (true)
		{
			InterlacedDoubleBinaryHeap::build(data, before_first, after_last,
					trusty_matrix, depth_matrix,
					dubious_min_nodes, dubious_max_nodes,
					next_dubious_min_nodes, next_dubious_max_nodes,
					min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);

			bool change_attempted = false;
			for (long depth = dubious_min_siblings.size() - 1; depth >= 0; depth -= 1)
			{
				if (dubious_min_siblings[depth].size() > 0 || dubious_max_siblings[depth].size() > 0)
				{
					change_attempted = true;
					int min_iterator = dubious_min_siblings[depth].size();
					int max_iterator = dubious_max_siblings[depth].size();
					while (min_iterator < max_iterator)
					{
						max_iterator -= 1;
						sort_max_siblings(data, before_first, after_last, dubious_max_siblings[depth][max_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					while (min_iterator > max_iterator)
					{
						min_iterator -= 1;
						sort_min_siblings(data, before_first, after_last, dubious_min_siblings[depth][min_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					while (min_iterator > 0)
					{
						min_iterator -= 1;
						max_iterator -= 1;
						sort_min_siblings(data, before_first, after_last, dubious_min_siblings[depth][min_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
						sort_max_siblings(data, before_first, after_last, dubious_max_siblings[depth][max_iterator], trusty_matrix, depth_matrix, dubious_min_nodes, dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
					}
					dubious_min_siblings[depth].clear();
					dubious_max_siblings[depth].clear();
				}
			}

			// copy dubious_min/max_nodes into dubious_min/max_siblings (attempt to avoid a memory leak)
			for (std::size_t depth = 0; depth < dubious_min_siblings.size(); depth += 1)
			{
				for (std::size_t min_iterator = 0; min_iterator < dubious_min_nodes[depth].size(); min_iterator += 1)
				{
					dubious_min_siblings[depth].push_back(dubious_min_nodes[depth][min_iterator]);
				}
				for (std::size_t max_iterator = 0; max_iterator < dubious_max_nodes[depth].size(); max_iterator += 1)
				{
					dubious_max_siblings[depth].push_back(dubious_max_nodes[depth][max_iterator]);
				}
			}

			if (!change_attempted)
			{
				break;
			}
		}
	}
}