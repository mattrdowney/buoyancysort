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

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long min_heapify(Type *data, long first, long after_last, long root)
	{
		long left = Heap::left_child(root, first-1); // NOTE: one-before-start intentionally
		if (left < after_last) // OPTIMIZATION: early return
		{
			long smallest = root;
			if (data[smallest] > data[left]) // OPTIMIZATION: left is smaller than (or equal to) right in sorted order
			{
				smallest = left;
			}
			long right = left + 1; // OPTIMIZATION: only works because first-1 is the pivot
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
		return root; // FIXME: eh, this isn't great. I have to decide how this works with out-of-bounds values (but also preserving behavior for the initial iteration).
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long max_heapify(Type *data, long first, long after_last, long root)
	{
		long right = Heap::right_child(root, after_last); // NOTE: signed type required or underflow can happen, NOTE: one-past-end intentionally
		if (first <= right) // OPTIMIZATION: early return
		{
			long largest = root;
			if (data[right] > data[largest]) // OPTIMIZATION: right is larger than (or equal to) left in sorted order
			{
				largest = right;
			}
			long left = right - 1; // OPTIMIZATION: only works because after_last is the pivot
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
		return root; // FIXME: eh, this isn't great. I have to decide how this works with out-of-bounds values (but also preserving behavior for the initial iteration).
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	void build_max_heap(Type *data, long first, long after_last)
	{
		long max_heapify_from = Heap::parent(first, after_last-1);
		while (max_heapify_from < after_last)
		{
			max_heapify(data, first, after_last, max_heapify_from);
			max_heapify_from += 1;
		}
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	void build_min_heap(Type *data, long first, long after_last)
	{
		long min_heapify_from = Heap::parent(after_last-1, first);
		while (min_heapify_from >= first)
		{
			min_heapify(data, first, after_last, min_heapify_from);
			min_heapify_from -= 1;
		}
	}
}