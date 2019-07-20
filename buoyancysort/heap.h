#pragma once

#include <stddef.h>
#include "power-of-two.h"

namespace Heap
{
	/// -1/0/+1 - based
	template <typename Type>
	Type parent(Type child, Type center)
	{
		return (child - center) / 2 + center;
	}

	/// -1/0/+1 - based
	template <typename Type>
	Type left_child(Type parent, Type center)
	{
		return (parent - center) * 2 - (parent <= center) + center;
	}

	/// -1/0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center)
	{
		return (parent - center) * 2 + (parent >= center) + center; 
	}

	template <typename Type>
	Type sibling(Type child, Type center)
	{
		// I can't think of how to remove branch prediction (the compiler is optimizing my code, but it doesn't seem to remove comparisons entirely)
		if (center - child > 1)
		{
			return child + ((center - child) % 2 == 0 ? -1 : +1);
		}
		if (center - child >= -1)
		{
			return -2 * (child - center) + child;
		}
		return child + ((child - center) % 2 == 0 ? +1 : -1);
	}

	template <typename Type>
	std::size_t min_heapify(Type *data, std::size_t first, std::size_t after_last, std::size_t root)
	{
		std::size_t left = Heap::left_child(root, first);
		if (left < after_last) // OPTIMIZATION: early return
		{
			std::size_t smallest = root;
			if (data[smallest] > data[left]) // OPTIMIZATION: left is smaller than (or equal to) right in sorted order
			{
				smallest = left;
			}
			std::size_t right = left + 1; // OPTIMIZATION: only works because first is the pivot
			if (right < after_last && data[smallest] > data[right])
			{
				smallest = right;
			}
			if (smallest != root)
			{
				std::swap(data[root], data[smallest]);
				return max_heapify(data, first, after_last, smallest); // NOTE: "smallest" has since changed.
			}
		}
		return root;
	}

	// TODO: VERIFY: does this sort of std::ptrdiff_t usage result in undefined behavior? 
	template <typename Type>
	std::ptrdiff_t max_heapify(Type *data, std::ptrdiff_t first, std::ptrdiff_t after_last, std::ptrdiff_t root)
	{
		std::ptrdiff_t right = Heap::right_child(root, after_last-1); // NOTE: std::ptrdiff_t (or signed type) required or underflow might occur
		if (first <= right) // OPTIMIZATION: early return
		{
			std::ptrdiff_t largest = root;
			if (data[right] > data[largest]) // OPTIMIZATION: right is larger than (or equal to) left in sorted order
			{
				largest = right;
			}
			std::ptrdiff_t left = right - 1; // OPTIMIZATION: only works because after_last-1 is the pivot
			if (first <= left && data[left] > data[largest])
			{
				largest = left;
			}
			if (largest != root)
			{
				std::swap(data[root], data[largest]);
				return max_heapify(data, first, after_last, largest); // NOTE: "largest" has since changed.
			}
		}
		return root;
	}

	template <typename Type>
	void build_max_heap(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t max_heapify_from = Heap::parent(first, after_last-1);
		while (max_heapify_from < after_last-1)
		{
			max_heapify(data, first, after_last, max_heapify_from);
			max_heapify_from += 1;
		}
	}

	template <typename Type>
	void build_min_heap(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t min_heapify_from = Heap::parent(after_last - 1, first);
		while (min_heapify_from > first)
		{
			min_heapify(data, first, after_last, min_heapify_from);
			min_heapify_from -= 1;
		}
	}
}