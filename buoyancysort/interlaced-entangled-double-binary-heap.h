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
		if (cached_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust))
		{
			long left_child = MinHeap::left_child(root, before_first);
			long right_child = left_child + 1;
			// Avoid a subtle bug when no children exist (or only one)
			bool invalid_left = (left_child < after_last && data[root] > data[left_child]);
			bool invalid_right = (right_child < after_last && data[root] > data[right_child]);
			if (invalid_left || invalid_right)
			{
				InterlacedDoubleBinaryHeap::set_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust, false);
				dubious_min_nodes[depth_matrix[root - before_first]].push_back(root);
				// TODO: topple all parent nodes
			}
		}
	}

	template <typename Type>
	void verify_max_stability(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
	}

	template <typename Type>
	void sort_min_siblings(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		// compare/sort siblings  (invalidating both sides of heap where necessary)
		// remember to update: 1) trusty_matrix (both sides), 2) dubious_min/max_nodes

		long left_child = MinHeap::left_child(root, before_first);
		long right_child = left_child + 1;
		if (data[left_child] > data[right_child])
		{
			std::swap(data[left_child], data[right_child]);
			if (cached_trust_min_node(before_first, after_last, trusty_matrix, right_child, min_right_line_of_implicit_trust))
			{
				long left_child_of_right_child = MinHeap::left_child(right_child, before_first);
				long right_child_of_right_child = left_child_of_right_child + 1;
				// Avoid a subtle bug when no children exist (or only one)
				bool invalid_left = (left_child_of_right_child < after_last && data[right_child] > data[left_child_of_right_child]);
				bool invalid_right = (right_child_of_right_child < after_last && data[right_child] > data[right_child_of_right_child]);
				if (invalid_left || invalid_right)
				{
					InterlacedDoubleBinaryHeap::set_trust_min_node(before_first, after_last, trusty_matrix, right_child, min_right_line_of_implicit_trust, false);
					dubious_min_nodes[depth_matrix[right_child - before_first]].push_back(right_child);
					// TODO: topple all parent nodes
				}
			}
			// look at two corresponding elements in max heap and consider toppling them
			/*
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
			*/
		}
	}

	template <typename Type>
	void sort_max_siblings(Type *data, long before_first, long after_last, long root, std::vector<bool> &trusty_matrix, std::vector<char> &depth_matrix, std::vector<std::vector<long>> &next_dubious_min_nodes, std::vector<std::vector<long>> &next_dubious_max_nodes, long min_right_line_of_implicit_trust, long max_left_line_of_implicit_trust)
	{
		// compare/sort siblings  (invalidating both sides of heap where necessary)
		// remember to update: 1) trusty_matrix (both sides), 2) dubious_min/max_nodes


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
			for (long depth = 0; depth < dubious_min_siblings.size(); depth += 1)
			{
				for (long min_iterator = 0; min_iterator < dubious_min_nodes[depth].size(); min_iterator += 1)
				{
					dubious_min_siblings.push_back(dubious_min_nodes[depth][min_iterator]);
				}
				for (long max_iterator = 0; max_iterator < dubious_max_nodes[depth].size(); max_iterator += 1)
				{
					dubious_max_siblings.push_back(dubious_max_nodes[depth][max_iterator]);
				}
			}

			if (!change_attempted)
			{
				break;
			}
		}
	}
}