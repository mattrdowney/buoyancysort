#pragma once

#include <stddef.h>

namespace MinHeap
{
	/// 0/+1 - based
	template <typename Type>
	Type parent(Type child, Type center, long tuple_size = 2)
	{
		return (child - center - 1) / tuple_size + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type left_child(Type parent, Type center, long tuple_size = 2)
	{
		return (parent - center) * tuple_size + center + 1;
	}

	//1 [2-6] [7-11] [12-16] [17-21] [22-26]

	/// 0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center, long tuple_size = 2)
	{
		return (parent - center) * tuple_size + center + tuple_size;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long heapify(Type *data, long before_first, long after_last, long root, long tuple_size = 2)
	{
		long left = left_child(root, before_first);
		if (left < after_last) // OPTIMIZATION: early return
		{
			long smallest = root;
			long right = std::min(right_child(root, before_first, tuple_size), after_last - 1);
			for (long child = left; child <= right; child += 1) // OPTIMIZATION: left is smaller than (or equal to) right in sorted order
			{
				smallest = (data[smallest] <= data[child] ? smallest : child);
			}
			if (smallest != root)
			{
				std::swap(data[root], data[smallest]);
				return heapify(data, before_first, after_last, smallest); // NOTE: "smallest" has since changed.
			}
		}
		return root;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	void build(Type *data, long before_first, long after_last, long tuple_size = 2)
	{
		long min_heapify_from = parent(after_last-1, before_first, tuple_size);
		while (min_heapify_from > before_first)
		{
			heapify(data, before_first, after_last, min_heapify_from, tuple_size);
			min_heapify_from -= 1;
		}
	}
}