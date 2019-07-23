#pragma once

#include <limits.h>
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
		std::set<long> dubious_min_nodes; // essentially a std::ordered_set<long> (not a std::unordered_set<long>)
		std::set<long> dubious_max_nodes;
		long dubious_node;
		long max_right_line_of_trust = MaxHeap::parent(before_first + 1, after_last); // when on a line of trust, a node is still trusted
		long max_left_line_of_trust = max_right_line_of_trust - 1;
		long min_left_line_of_trust = MinHeap::parent(after_last - 1, before_first);
		long min_right_line_of_trust = min_left_line_of_trust + 1;
		while (max_right_line_of_trust < after_last)
		{

			dubious_node = MaxHeap::heapify(data, before_first, after_last, max_right_line_of_trust);
			max_right_line_of_trust += 1;
			get_dubious_min_heap_elements(data, before_first, after_last, dubious_min_nodes, dubious_node, min_left_line_of_trust, min_right_line_of_trust, max_left_line_of_trust, max_right_line_of_trust);

			dubious_node = MinHeap::heapify(data, before_first, after_last, min_left_line_of_trust);
			min_left_line_of_trust -= 1;
			get_dubious_max_heap_elements(data, before_first, after_last, dubious_max_nodes, dubious_node, min_left_line_of_trust, min_right_line_of_trust, max_left_line_of_trust, max_right_line_of_trust);

			verify(data, before_first, after_last, dubious_min_nodes, dubious_max_nodes, min_left_line_of_trust, min_right_line_of_trust, max_left_line_of_trust, max_right_line_of_trust);
		}

		std::cout << std::endl;
		for (long index : dubious_min_nodes)
		{
			std::cout << index << " ";
		}
		std::cout << std::endl;
	}

	enum class AdjustType { Undecided, Min, Max };

	template <typename Type>
	void verify(Type *data, long before_first, long after_last, std::set<long> &dubious_min_nodes, std::set<long> &dubious_max_nodes, long min_left_line_of_trust, long min_right_line_of_trust, long max_left_line_of_trust, long max_right_line_of_trust)
	{
		while (dubious_min_nodes.size() > 0 || dubious_max_nodes.size() > 0)
		{
			Print::print(data, before_first, after_last);

			AdjustType mode = AdjustType::Undecided;

			// TODO: std::set has O(k) performance here but I don't care yet.
			if (dubious_min_nodes.size() > 0)
			{
				mode = AdjustType::Min;
			}

			if (dubious_max_nodes.size() > 0)
			{
				if (mode == AdjustType::Undecided)
				{
					mode = AdjustType::Max;
				}
				else
				{
					long max_distance = (*dubious_max_nodes.begin() - before_first);
					long min_distance = (after_last - *dubious_min_nodes.rbegin());
					mode = (max_distance < min_distance ? AdjustType::Max : AdjustType::Min);
				}
			}
			
			if (mode == AdjustType::Min)
			{
				long adjust_index = *dubious_min_nodes.rbegin();
				std::cout << std::endl << "Min: " << adjust_index << std::endl;
				dubious_min_nodes.erase(adjust_index);
				long dubious_node = MinHeap::heapify(data, before_first, after_last, adjust_index);
				get_dubious_max_heap_elements(data, before_first, after_last, dubious_max_nodes, dubious_node, dubious_node, min_right_line_of_trust, max_left_line_of_trust, max_right_line_of_trust);
			}
			else // if (mode == AdjustType.Max)
			{
				long adjust_index = *dubious_max_nodes.begin();
				std::cout << std::endl << "Max: " << adjust_index << std::endl;
				dubious_max_nodes.erase(adjust_index);
				long dubious_node = MaxHeap::heapify(data, before_first, after_last, adjust_index);
				get_dubious_min_heap_elements(data, before_first, after_last, dubious_min_nodes, dubious_node, min_left_line_of_trust, min_right_line_of_trust, max_left_line_of_trust, dubious_node);
			}
		}
	}

	template <typename Type>
	void get_dubious_min_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_min_nodes, long dubious_node, long min_left_line_of_trust, long min_right_line_of_trust, long max_left_line_of_trust, long max_right_line_of_trust)
	{
		while (dubious_node < max_right_line_of_trust)
		{
			if (min_left_line_of_trust < dubious_node && dubious_node < min_right_line_of_trust)
			{
				dubious_min_nodes.insert(dubious_node);
			}
			dubious_node = MaxHeap::parent(dubious_node, after_last);
		}
	}

	template <typename Type>
	void get_dubious_max_heap_elements(Type *data, long before_first, long after_last, std::set<long> &dubious_max_nodes, long dubious_node, long min_left_line_of_trust, long min_right_line_of_trust, long max_left_line_of_trust, long max_right_line_of_trust)
	{
		while (dubious_node > min_left_line_of_trust)
		{
			if (max_left_line_of_trust < dubious_node && dubious_node < max_right_line_of_trust)
			{
				dubious_max_nodes.insert(dubious_node);
			}
			dubious_node = MinHeap::parent(dubious_node, before_first);
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