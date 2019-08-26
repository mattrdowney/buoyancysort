#pragma once

#include <algorithm>
#include <stddef.h>

namespace MaxHeap
{
	/// -1/0 - based
	template <typename Type>
	Type parent(Type child, Type center, long tuple_size = 2)
	{
		return (child - center) / tuple_size + center;
	}

	/// -1/0 - based
	template <typename Type>
	Type left_child(Type parent, Type center, long tuple_size = 2)
	{
		return (parent - center) * tuple_size + center - (tuple_size - 1);
	}

	/// -1/0 - based
	template <typename Type>
	Type right_child(Type parent, Type center, long tuple_size = 2)
	{
		return (parent - center) * tuple_size + center;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long heapify(Type *data, long before_first, long after_last, long root, long tuple_size = 2)
	{
		long right = right_child(root, after_last, tuple_size); // NOTE: signed type required or underflow can happen
		if (before_first < right) // OPTIMIZATION: early return
		{
			long largest = root;
			long left = std::max(left_child(root, after_last, tuple_size), before_first + 1);
			for (long child = right; child >= left; child -= 1) // OPTIMIZATION: right is larger than (or equal to) left in sorted order
			{
				largest = (data[largest] >= data[child] ? largest : child);
			}
			if (largest != root)
			{
				std::swap(data[root], data[largest]);
				return heapify(data, before_first, after_last, largest); // NOTE: "largest" has since changed.
			}
		}
		return root;
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	void build(Type *data, long before_first, long after_last, long tuple_size = 2)
	{
		long max_heapify_from = parent(before_first+1, after_last, tuple_size);
		while (max_heapify_from < after_last)
		{
			heapify(data, before_first, after_last, max_heapify_from, tuple_size);
			max_heapify_from += 1;
		}
	}
}