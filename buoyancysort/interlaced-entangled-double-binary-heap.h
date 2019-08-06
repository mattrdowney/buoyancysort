#pragma once

#include <vector>
#include "interlaced-double-binary-heap.h"
#include "min-heap.h"
#include "max-heap.h"

// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
namespace InterlacedEntangledDoubleBinaryHeap // even if this doesn't work, it's still interesting to develop
{
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
						//sort_max_siblings()
					}
					while (min_iterator > max_iterator)
					{
						min_iterator -= 1;
						//sort_min_siblings()
					}
					while (min_iterator > 0)
					{
						min_iterator -= 1;
						max_iterator -= 1;
						//sort_min_siblings()
						//sort_max_siblings()
					}
					dubious_min_siblings[depth].clear();
					dubious_max_siblings[depth].clear();
				}
			}

			// compare/sort siblings  (invalidating both sides of heap where necessary)
			// remember to update: 1) trusty_matrix (both sides), 2) dubious_min/max_nodes, 3) [at the end] dubious_min/max_siblings

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