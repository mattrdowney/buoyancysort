#pragma once

#include <stddef.h>

namespace MaxHeap
{
	/// -1/0 - based
	template <typename Type>
	Type parent(Type child, Type center, long tuple_size = 2)
	{
		return (child - center + 2) / tuple_size + center - 1;
	}

	/// -1/0 - based
	template <typename Type>
	Type left_child(Type parent, Type center, long tuple_size = 2)
	{
		return (parent - center) * tuple_size + center - 1;
	}

	/// -1/0 - based
	template <typename Type>
	Type right_child(Type parent, Type center, long tuple_size = 2)
	{
		return left_child(parent, center, tuple_size) + (tuple_size - 1);
	}

	// WARNING: A signed type is neccessary here.
	template <typename Type>
	long heapify(Type *data, long before_first, long after_last, long root)
	{
		long right = right_child(root, after_last); // NOTE: signed type required or underflow can happen
		if (before_first < right) // OPTIMIZATION: early return
		{
			long largest = root;
			if (data[right] > data[largest]) // OPTIMIZATION: right is larger than (or equal to) left in sorted order
			{
				largest = right;
			}
			long left = right - 1;
			if (before_first < left && data[left] > data[largest])
			{
				largest = left;
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
	void build(Type *data, long before_first, long after_last)
	{
		long max_heapify_from = parent(before_first+1, after_last);
		while (max_heapify_from < after_last)
		{
			heapify(data, before_first, after_last, max_heapify_from);
			max_heapify_from += 1;
		}
	}
}