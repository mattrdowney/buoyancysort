#pragma once

#include <limits.h>
#include <set>
#include <stddef.h>
#include <string.h>
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

	enum class AdjustType { Undecided, Min, Max };

	template <typename Type>
	void rebuild(Type *data, long before_first, long after_last, std::set<long> &dubious_min_nodes, std::set<long> &dubious_max_nodes)
	{
		Print::print(data, before_first, after_last);

		enhance_dubious_min_heap_elements(data, before_first, after_last, dubious_min_nodes);
		enhance_dubious_max_heap_elements(data, before_first, after_last, dubious_max_nodes);

		std::set<long> next_dubious_min_nodes{ before_first + 1 }; // sentinel nodes, which simplify logic (in my opinion)
		std::set<long> next_dubious_max_nodes{ after_last - 1 };

		std::set<long>::reverse_iterator min_iterator = dubious_min_nodes.rbegin();
		std::set<long>::iterator max_iterator = dubious_max_nodes.begin();

		long max_right_line_of_trust = MaxHeap::parent(*max_iterator, after_last); // when on a line of trust, a node is still trusted
		long max_left_line_of_trust = MaxHeap::parent(before_first + 1, after_last) - 1;
		long min_left_line_of_trust = MinHeap::parent(*min_iterator, before_first);
		long min_right_line_of_trust = MinHeap::parent(after_last - 1, before_first) + 1;

		while (min_iterator != dubious_min_nodes.rend() || max_iterator != dubious_max_nodes.end())
		{
			AdjustType mode = AdjustType::Undecided;

			// TODO: std::set has O(k) performance here but I don't care yet.
			if (min_iterator != dubious_min_nodes.rend())
			{
				mode = AdjustType::Min;
			}

			if (max_iterator != dubious_max_nodes.end())
			{
				if (mode == AdjustType::Undecided)
				{
					mode = AdjustType::Max;
				}
				else
				{
					long max_distance = (*max_iterator - before_first);
					long min_distance = (after_last - *min_iterator);
					mode = (max_distance < min_distance ? AdjustType::Max : AdjustType::Min);
				}
			}

			if (mode == AdjustType::Min)
			{
				long adjust_index = *min_iterator;
				std::cout << std::endl << "Min: " << adjust_index << std::endl;
				min_iterator++;
				if (min_iterator != dubious_min_nodes.rend())
				{
					min_left_line_of_trust = *min_iterator;
				}
				long dubious_node = MinHeap::heapify(data, before_first, after_last, adjust_index);
				get_dubious_max_heap_elements(data, before_first, after_last, next_dubious_max_nodes, dubious_node, dubious_node, min_right_line_of_trust, max_left_line_of_trust, max_right_line_of_trust);
			}
			else // if (mode == AdjustType.Max)
			{
				long adjust_index = *max_iterator;
				std::cout << std::endl << "Max: " << adjust_index << std::endl;
				max_iterator++;
				if (max_iterator != dubious_max_nodes.end())
				{
					max_right_line_of_trust = *max_iterator;
				}
				long dubious_node = MaxHeap::heapify(data, before_first, after_last, adjust_index);
				get_dubious_min_heap_elements(data, before_first, after_last, next_dubious_min_nodes, dubious_node, min_left_line_of_trust, min_right_line_of_trust, max_left_line_of_trust, dubious_node);
			}
		}

		if (next_dubious_min_nodes.size() > 0 || next_dubious_max_nodes.size() > 0)
		{
			rebuild(data, before_first, after_last, next_dubious_min_nodes, next_dubious_max_nodes);
		}
	}

	template <typename Type>
	void build(Type *data, long before_first, long after_last)
	{
		// TODO: A lot of this boilerplate will be repeated later. How can I get around that?
		long bitset_size = after_last - before_first + 1;
		long heap_depth = bitset_size; // FIXME:
		std::vector<bool> dubious_min_matrix(bitset_size); // To index into this array, always subtract before_first.
		std::vector<bool> dubious_max_matrix(bitset_size); // Cannot use a c-style array unfortunately -- for clarity of static size
		std::vector<char> min_depth_matrix(bitset_size); // NOTE: Theoretically, this *could* fail with certain char/long definitions (sort of undefined behavior)
		std::vector<char> min_depth_matrix(bitset_size);
		long array_index = 0;
		long memset_index_begin = 1;
		long memset_index_end = memset_index_begin + 1;
		while (memset_index_begin < after_last)
		{
			long end = min(after_last, memset_index_end);
			memset(min_depth_matrix.data() + memset_index_begin, array_index, end - memset_index_begin);
			memset(min_depth_matrix.data() + after_last - (end - memset_index_begin + 1), array_index, end - memset_index_begin);
			array_index += 1;
			memset_index_begin *= 2;
			memset_index_end *= 2;
		}ss
		// TODO: set indices 0, 1, n-2, n-1 to dubious (as sentinel nodes) -- and insert "1", "n-2" into dubious nodes.
		std::vector<std::vector<long>> dubious_min_nodes(heap_depth);
		std::vector<std::vector<long>> dubious_max_nodes(heap_depth);
		// TODO: Because of the nature of next_power_of_two() (time complexity ~O(lg(lg(n))) I think), I think I have to cache the powers_of_two that are used in dubious_min/max_nodes.
		long dubious_node;
		long max_right_line_of_explicit_trust = MaxHeap::parent(before_first + 1, after_last); // when on a line of trust, a node is still trusted
		long max_left_line_of_implicit_trust = max_right_line_of_explicit_trust - 1;
		long min_left_line_of_explicit_trust = MinHeap::parent(after_last - 1, before_first);
		long min_right_line_of_implicit_trust = min_left_line_of_explicit_trust + 1;
		while (max_right_line_of_explicit_trust < after_last)
		{
			dubious_node = MaxHeap::heapify(data, before_first, after_last, max_right_line_of_explicit_trust);
			max_right_line_of_explicit_trust += 1;
			get_dubious_min_heap_elements(data, before_first, after_last, dubious_min_nodes, dubious_node, min_left_line_of_explicit_trust, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust, max_right_line_of_explicit_trust);

			dubious_node = MinHeap::heapify(data, before_first, after_last, min_left_line_of_explicit_trust);
			min_left_line_of_explicit_trust -= 1;
			get_dubious_max_heap_elements(data, before_first, after_last, dubious_max_nodes, dubious_node, min_left_line_of_explicit_trust, min_right_line_of_implicit_trust, max_left_line_of_implicit_trust, max_right_line_of_explicit_trust);
		}

		std::cout << std::endl;
		for (long index : dubious_min_nodes)
		{
			std::cout << index << " ";
		}
		for (long index : dubious_max_nodes)
		{
			std::cout << index << " ";
		}
		std::cout << std::endl;

		rebuild(data, before_first, after_last, dubious_min_nodes, dubious_max_nodes);
	}

	// use min_heapify() and max_heapify() in an alternating fasion (as subroutines).
	// when the return isn't the index, swaps were made and you are given a descendent child.
	// Find the list of decendents all the way up until the ancestor.
	// Add that list of decendents to a std::ordered_set<std::size_t> of dubious_nodes
	// At least when those nodes are past the "lines of trust".
	// TODO: RESEARCH: how do "lines of trust" work? When are they invalidated? When are they still valid/"trusty"?
	// NOTE: "dubious" and "trusty" are references to Dijkstra's smoothsort.
}