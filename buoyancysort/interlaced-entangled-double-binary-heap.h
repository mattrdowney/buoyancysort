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
		if (root < min_right_line_of_implicit_trust && InterlacedDoubleBinaryHeap::cached_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust))
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
		if (root > max_left_line_of_implicit_trust && InterlacedDoubleBinaryHeap::cached_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust))
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
					next_dubious_max_nodes[depth_matrix[after_last - topple_cursor]].push_back(topple_cursor);
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
			verify_max_stability(data, before_first, after_last, left_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
		}
		/*
		if (InterlacedDoubleBinaryHeap::test_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust))
		{
			next_dubious_min_nodes[depth_matrix[root - before_first]].push_back(root);
			InterlacedDoubleBinaryHeap::set_trust_min_node(before_first, after_last, trusty_matrix, root, min_right_line_of_implicit_trust, false);
		}
		*/
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
			verify_min_stability(data, before_first, after_last, right_child, trusty_matrix, depth_matrix, next_dubious_min_nodes, next_dubious_max_nodes, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);
		}
		/*
		if (InterlacedDoubleBinaryHeap::test_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust))
		{
			next_dubious_max_nodes[depth_matrix[after_last - root]].push_back(root);
			InterlacedDoubleBinaryHeap::set_trust_max_node(before_first, after_last, trusty_matrix, root, max_left_line_of_implicit_trust, false);
		}
		*/
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
		// OPTIMIZE: (blah blah blah) they're "Aligned"; you're doing twice the work.

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

			// Extend the definition of dubious_min/max_siblings to include "implicitly trusted" nodes.
			// You can do this by going through the lowest "depths" and seeing if there are any children in that region
			// I think as long as my implementation is only a little dumb, it should not change the time complexity beyond a constant factor
			// (e.g. I have to check two leaf/child nodes, but that could only multiply the time complexity by 3 in the worst case.)

			if (!change_attempted)
			{
				break;
			}
		}

		// The fix isn't quite this easy
		/*
		// The implicitly trusted nodes may not fulfill the "Alignment" property, so sort them.
		for (long left_sibling = before_first + 2; left_sibling + 1 < after_last; left_sibling += 2)
		{
			long right_sibling = left_sibling + 1;
			if (data[left_sibling] > data[right_sibling])
			{
				std::swap(data[left_sibling], data[right_sibling]);
			}
		}
		*/
	}
}