#pragma once

#include <stddef.h>

namespace MinHeap
{
	/// 0/+1 - based
	template <typename Type>
	Type parent(Type child, Type center)
	{
		return (child - center) / 2 + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type left_child(Type parent, Type center)
	{
		return (parent - center) * 2 + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center)
	{
		return left_child(parent, center) + 1;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long heapify(Type *data, long first, long after_last, long root)
	{
		long left = left_child(root, first);
		if (left < after_last) // OPTIMIZATION: early return
		{
			long smallest = root;
			if (data[smallest] > data[left]) // OPTIMIZATION: left is smaller than (or equal to) right in sorted order
			{
				smallest = left;
			}
			long right = left + 1;
			if (right < after_last && data[smallest] > data[right])
			{
				smallest = right;
			}
			if (smallest != root)
			{
				std::swap(data[root], data[smallest]);
				return heapify(data, first, after_last, smallest); // NOTE: "smallest" has since changed.
			}
		}
		return root;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	void build_heap(Type *data, long first, long after_last)
	{
		long min_heapify_from = parent(after_last - 1, first);
		while (min_heapify_from >= first)
		{
			heapify(data, first, after_last, min_heapify_from);
			min_heapify_from -= 1;
		}
	}
}