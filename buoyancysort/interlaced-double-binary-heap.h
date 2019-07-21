#pragma once

#include <set>
#include <stddef.h>
#include "heap.h"
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
	void build(Type *data, long before_first, long after_last)
	{
		std::set<long> dubious_nodes; // essentially a std::ordered_set<long> (not a std::unordered_set<long>)
		long dubious_node;
		long max_right_line_of_trust = MaxHeap::parent(before_first + 1, after_last); // when on a line of trust, a node is still trusted
		long max_left_line_of_trust = max_right_line_of_trust - 1;
		long min_left_line_of_trust = MinHeap::parent(after_last - 1, before_first);
		long min_right_line_of_trust = min_left_line_of_trust + 1;
		while (max_right_line_of_trust < after_last)
		{
			dubious_node = MaxHeap::heapify(data, before_first, after_last, max_right_line_of_trust);
			while (dubious_node < max_right_line_of_trust)
			{
				if (min_left_line_of_trust < dubious_node && dubious_node < min_right_line_of_trust)
				{
					dubious_nodes.insert(dubious_node); // NOTE: invalidation can happen with parent or children
				}
				dubious_node = MaxHeap::parent(dubious_node, after_last);
			}
			dubious_node = MinHeap::heapify(data, before_first, after_last, min_left_line_of_trust);
			while (dubious_node > min_left_line_of_trust)
			{
				if (max_left_line_of_trust < dubious_node && dubious_node < max_right_line_of_trust)
				{
					dubious_nodes.insert(dubious_node); // NOTE: invalidation can happen with parent or children
				}
				dubious_node = MinHeap::parent(dubious_node, before_first);
			}
			max_right_line_of_trust += 1;
			min_left_line_of_trust -= 1;
		}

		std::cout << std::endl;
		for (long index : dubious_nodes)
		{
			std::cout << index << " ";
		}
		std::cout << std::endl;
		
		build(data, before_first, after_last, dubious_nodes);
	}

	template <typename Type>
	void build(Type *data, long before_first, long after_last, std::set<long> &known_dubious_nodes)
	{
		if (known_dubious_nodes.size() > 0)
		{
			long max_right_line_of_trust = MaxHeap::parent(known_dubious_nodes[0], after_last); // when on a line of trust, a node is still trusted
			long max_left_line_of_trust = MaxHeap::parent(before_first+1, after_last)-1;
			long min_left_line_of_trust = MinHeap::parent(known_dubious_nodes[known_dubious_nodes.size() - 1], before_first);
			long min_right_line_of_trust = MinHeap::parent(after_last-1, before_first)+1;

			std::set<long> dubious_nodes; // essentially a std::ordered_set<long> (not a std::unordered_set<long>)
										  // The dubious nodes must include all of the parents of dubious nodes (in both directions), *except* those before known_dubious_nodes[0] (or after known_dubious_nodes[size-1]). 
			long dubious_node;

			// Compute dubious nodes
			known_dubious_nodes.clear();

			// Refactor to step through a set<long> as a forwards or backwards in-order traversal (but also with a fair, alternating strategy)

			while (max_right_line_of_trust < after_last)
			{
				dubious_node = MaxHeap::heapify(data, before_first, after_last, max_right_line_of_trust);
				while (dubious_node < max_right_line_of_trust)
				{
					if (min_left_line_of_trust < dubious_node && dubious_node < min_right_line_of_trust)
					{
						known_dubious_nodes.insert(dubious_node); // NOTE: invalidation can happen with parent or children
					}
					dubious_node = MaxHeap::parent(dubious_node, after_last);
				}
				dubious_node = MinHeap::heapify(data, before_first, after_last, min_left_line_of_trust);
				while (dubious_node > min_left_line_of_trust)
				{
					if (max_left_line_of_trust < dubious_node && dubious_node < max_right_line_of_trust)
					{
						known_dubious_nodes.insert(dubious_node); // NOTE: invalidation can happen with parent or children
					}
					dubious_node = MinHeap::parent(dubious_node, before_first);
				}
				max_right_line_of_trust += 1;
				min_left_line_of_trust -= 1;
			}

			std::cout << std::endl;
			for (long index : known_dubious_nodes)
			{
				std::cout << index << " ";
			}
			std::cout << std::endl;

			build(data, before_first, after_last, known_dubious_nodes);
		}
	}

	// use min_heapify() and max_heapify() in an alternating fasion (as subroutines).
	// when the return isn't the index, swaps were made and you are given a descendent child.
	// Find the list of decendents all the way up until the ancestor.
	// Add that list of decendents to a std::ordered_set<std::size_t> of dubious_nodes
	// At least when those nodes are past the "lines of trust".
	// TODO: RESEARCH: how do "lines of trust" work? When are they invalidated? When are they still valid/"trusty"?
	// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
}