#pragma once

#include <algorithm>
#include <assert.h>
#include <iostream>
#include "min-heap.h"
#include "max-heap.h"
#include "median-heap.h"

namespace HeapTests // The amount of namespaces is basically parody at this point
{
	template <typename Type>
	void test_min_heap(Type *data, long before_first, long after_last, long parent, long tuple_size)
	{
		long left_child = MinHeap::left_child(parent, before_first, tuple_size);
		long right_child = std::min(MinHeap::right_child(parent, before_first, tuple_size), after_last - 1);
		for (int child = left_child; child <= right_child; child += 1)
		{
			if (data[parent] > data[child])
			{
				std::cout << "[ " << data[parent] << " (@" << parent << ") > " << data[child] << " (@" << child << ") ], ";
			}
			test_min_heap(data, before_first, after_last, child, tuple_size);
		}
	}

	template <typename Type>
	void test_min_heap(Type *data, long before_first, long after_last, long tuple_size)
	{
		std::cout << std::endl;
		if (before_first + 1 < after_last)
		{
			test_min_heap(data, before_first, after_last, before_first + 1, tuple_size);
		}
		std::cout << std::endl;
	}

	template <typename Type>
	void test_min_heap_alignment(Type *data, long before_first, long after_last, long parent, long tuple_size)
	{
		long left_child = MinHeap::left_child(parent, before_first, tuple_size);
		long right_child = std::min(MinHeap::right_child(parent, before_first, tuple_size), after_last - 1);
		for (int child = left_child; child <= right_child; child += 1)
		{
			if (child != right_child && data[child] > data[child + 1])
			{
				std::cout << "[ " << data[child] << " (@" << child << ") > " << data[child + 1] << " (@" << (child + 1) << ") ], ";
			}
			test_min_heap_alignment(data, before_first, after_last, child, tuple_size);
		}
	}

	template <typename Type>
	void test_min_heap_alignment(Type *data, long before_first, long after_last, long tuple_size)
	{
		std::cout << std::endl;
		if (before_first + 1 < after_last)
		{
			test_min_heap_alignment(data, before_first, after_last, before_first + 1, tuple_size);
		}
		std::cout << std::endl;
	}

	template <typename Type>
	void test_max_heap(Type *data, long before_first, long after_last, long parent, long tuple_size)
	{
		long left_child = std::max(MaxHeap::left_child(parent, after_last, tuple_size), before_first + 1);
		long right_child = MaxHeap::right_child(parent, after_last, tuple_size);
		for (long child = right_child; child >= left_child && child > before_first; child -= 1)
		{
			if (data[child] > data[parent])
			{
				std::cout << "[ " << data[child] << " (@" << child << ") > " << data[parent] << " (@" << parent << ") ], ";
			}
			test_max_heap(data, before_first, after_last, child, tuple_size);
		}
	}

	template <typename Type>
	void test_max_heap(Type *data, long before_first, long after_last, long tuple_size)
	{
		std::cout << std::endl;
		if (before_first < after_last - 1)
		{
			test_max_heap(data, before_first, after_last, after_last - 1, tuple_size);
		}
		std::cout << std::endl;
	}

	// In theory max heap alignment is the same as min heap alignment but extra tests are generally good
	template <typename Type>
	void test_max_heap_alignment(Type *data, long before_first, long after_last, long parent, long tuple_size)
	{
		long left_child = std::max(MaxHeap::left_child(parent, after_last, tuple_size), before_first + 1);
		long right_child = MaxHeap::right_child(parent, after_last, tuple_size);
		for (int child = right_child; child >= left_child; child -= 1)
		{
			if (child != left_child && data[child - 1] > data[child])
			{
				std::cout << "[ " << data[child - 1] << " (@" << (child - 1) << ") > " << data[child] << " (@" << child << ") ], ";
			}
			test_max_heap_alignment(data, before_first, after_last, child, tuple_size);
		}
	}

	template <typename Type>
	void test_max_heap_alignment(Type *data, long before_first, long after_last, long tuple_size)
	{
		std::cout << std::endl;
		if (before_first < after_last - 1)
		{
			test_max_heap_alignment(data, before_first, after_last, after_last - 1, tuple_size);
		}
		std::cout << std::endl;
	}

	void heap_tests()
	{
	//	// Parent, unshifted
	//	assert(Heap::parent(-4,  0) == -2);
	//	assert(Heap::parent(-3,  0) == -1);
	//	assert(Heap::parent(-2,  0) == -1);
	//	assert(Heap::parent(-1,  0) ==  0);
	//	assert(Heap::parent( 0,  0) ==  0);
	//	assert(Heap::parent(+1,  0) ==  0);
	//	assert(Heap::parent(+2,  0) == +1);
	//	assert(Heap::parent(+3,  0) == +1);
	//	assert(Heap::parent(+4,  0) == +2);

	//	// Parent, shifted
	//	assert(Heap::parent( 0, +4) == +2);
	//	assert(Heap::parent(+1, +4) == +3);
	//	assert(Heap::parent(+2, +4) == +3);
	//	assert(Heap::parent(+3, +4) == +4);
	//	assert(Heap::parent(+4, +4) == +4);
	//	assert(Heap::parent(+5, +4) == +4);
	//	assert(Heap::parent(+6, +4) == +5);
	//	assert(Heap::parent(+7, +4) == +5);
	//	assert(Heap::parent(+8, +4) == +6);

	//	// Left child, unshifted
	//	assert(Heap::left_child(-4,  0) == -9);
	//	assert(Heap::left_child(-3,  0) == -7);
	//	assert(Heap::left_child(-2,  0) == -5);
	//	assert(Heap::left_child(-1,  0) == -3);
	//	assert(Heap::left_child( 0,  0) == -1);
	//	assert(Heap::left_child(+1,  0) == +2);
	//	assert(Heap::left_child(+2,  0) == +4);
	//	assert(Heap::left_child(+3,  0) == +6);
	//	assert(Heap::left_child(+4,  0) == +8);

	//	// Left child, shifted
	//	assert(Heap::left_child( 0, +4) == -5);
	//	assert(Heap::left_child(+1, +4) == -3);
	//	assert(Heap::left_child(+2, +4) == -1);
	//	assert(Heap::left_child(+3, +4) == +1);
	//	assert(Heap::left_child(+4, +4) == +3);
	//	assert(Heap::left_child(+5, +4) == +6);
	//	assert(Heap::left_child(+6, +4) == +8);
	//	assert(Heap::left_child(+7, +4) == +10);
	//	assert(Heap::left_child(+8, +4) == +12);

	//	// Right child, unshifted
	//	assert(Heap::right_child(-4,  0) == -8);
	//	assert(Heap::right_child(-3,  0) == -6);
	//	assert(Heap::right_child(-2,  0) == -4);
	//	assert(Heap::right_child(-1,  0) == -2);
	//	assert(Heap::right_child( 0,  0) == +1);
	//	assert(Heap::right_child(+1,  0) == +3);
	//	assert(Heap::right_child(+2,  0) == +5);
	//	assert(Heap::right_child(+3,  0) == +7);
	//	assert(Heap::right_child(+4,  0) == +9);

	//	// Right child, shifted
	//	assert(Heap::right_child( 0, +4) == -4);
	//	assert(Heap::right_child(+1, +4) == -2);
	//	assert(Heap::right_child(+2, +4) ==  0);
	//	assert(Heap::right_child(+3, +4) == +2);
	//	assert(Heap::right_child(+4, +4) == +5);
	//	assert(Heap::right_child(+5, +4) == +7);
	//	assert(Heap::right_child(+6, +4) == +9);
	//	assert(Heap::right_child(+7, +4) == +11);
	//	assert(Heap::right_child(+8, +4) == +13);

	//	// Sibling, unshifted
	//	assert(Heap::sibling(-4, 0) == -5);
	//	assert(Heap::sibling(-3, 0) == -2);
	//	assert(Heap::sibling(-2, 0) == -3);
	//	assert(Heap::sibling(-1, 0) == +1);
	//	assert(Heap::sibling(0, 0) == 0);
	//	assert(Heap::sibling(+1, 0) == -1);
	//	assert(Heap::sibling(+2, 0) == +3);
	//	assert(Heap::sibling(+3, 0) == +2);
	//	assert(Heap::sibling(+4, 0) == +5);

	//	// Sibling, shifted
	//	assert(Heap::sibling(0, +4) == -1);
	//	assert(Heap::sibling(+1, +4) == +2);
	//	assert(Heap::sibling(+2, +4) == +1);
	//	assert(Heap::sibling(+3, +4) == +5);
	//	assert(Heap::sibling(+4, +4) == +4);
	//	assert(Heap::sibling(+5, +4) == +3);
	//	assert(Heap::sibling(+6, +4) == +7);
	//	assert(Heap::sibling(+7, +4) == +6);
	//	assert(Heap::sibling(+8, +4) == +9);
	}

	
}