#pragma once

#include <vector>
#include "interlaced-double-binary-heap.h"
#include "min-heap.h"
#include "max-heap.h"

// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
namespace InterlacedEntangledDoubleBinaryHeap // even if this doesn't work, it's still interesting to develop
{
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

		InterlacedDoubleBinaryHeap::initialize(before_first, after_last,
				trusty_matrix, depth_matrix,
				dubious_min_nodes, dubious_max_nodes,
				next_dubious_min_nodes, next_dubious_max_nodes,
				min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);

		std::vector<std::vector<long>> dubious_min_siblings(dubious_min_nodes); // Attempting deep copy, which is important because this...
		std::vector<std::vector<long>> dubious_max_siblings(dubious_max_nodes); // ... is a vector<vector>(the inner vectors are .clear()'ed).

		while (true)
		{
			InterlacedDoubleBinaryHeap::build(data, before_first, after_last,
					trusty_matrix, depth_matrix,
					dubious_min_nodes, dubious_max_nodes,
					next_dubious_min_nodes, next_dubious_max_nodes,
					min_right_line_of_implicit_trust, max_left_line_of_implicit_trust);

			bool change_attempted = false;

			// compare/sort siblings  (invalidating both sides of heap where necessary)
			// remember to update: 1) trusty_matrix (both sides), 2) dubious_min/max_nodes, 3) [at the end] dubious_min/max_siblings

			if (!change_attempted)
			{
				break;
			}
		}
	}
}