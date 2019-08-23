#pragma once

#include <algorithm>
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
	long heapify(Type *data, long before_first, long after_last, long root, long tuple_size = 2)
	{
		long right = right_child(root, after_last, tuple_size); // NOTE: signed type required or underflow can happen
		if (before_first < right) // OPTIMIZATION: early return
		{
			long largest = root;
			if (data[right] > data[largest]) // OPTIMIZATION: right is larger than (or equal to) left in sorted order
			{
				largest = right;
			}
			long left = std::max(left_child(root, after_last, tuple_size), before_first + 1);
			for (long sibling = right - 1; sibling > left; sibling -= 2) // amusing "OPTIMIZATION": compare pairs of siblings to ~ halve the number of comparisons; you can create a comparison tree (but that requires extra overhead).
			{
				long largest_sibling = (data[sibling - 1] > data[sibling] ? sibling - 1 : sibling);
				largest = (data[largest] > data[largest_sibling] ? largest : largest_sibling);
			}
			if ((tuple_size % 2) == 0) // You have an extra sibling that has to be compared with a little extra overhead.
			{
				largest = (data[largest] > data[left] ? largest : left);
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