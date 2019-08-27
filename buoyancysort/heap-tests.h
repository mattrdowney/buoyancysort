#pragma once

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <random>
#include "fractal-max-heap.h"
#include "fractal-min-heap.h"
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
		// 2-ary heap tests
		// Parent, unshifted
		assert(MinHeap::parent( 0, -1) == -1);
		assert(MinHeap::parent(+1, -1) ==  0);
		assert(MinHeap::parent(+2, -1) ==  0);
		assert(MinHeap::parent(+3, -1) == +1);

		// Parent, shifted
		assert(MaxHeap::parent( 0, +4) == +2);
		assert(MaxHeap::parent(+1, +4) == +3);
		assert(MaxHeap::parent(+2, +4) == +3);
		assert(MaxHeap::parent(+3, +4) == +4);

		// Left child, unshifted
		assert(MinHeap::left_child( 0, -1) == +1);
		assert(MinHeap::left_child(+1, -1) == +3);
		assert(MinHeap::left_child(+2, -1) == +5);
		assert(MinHeap::left_child(+3, -1) == +7);

		// Left child, shifted
		assert(MaxHeap::left_child( 0, +4) == -5);
		assert(MaxHeap::left_child(+1, +4) == -3);
		assert(MaxHeap::left_child(+2, +4) == -1);
		assert(MaxHeap::left_child(+3, +4) == +1);

		// Right child, unshifted
		assert(MinHeap::right_child( 0, -1) == +2);
		assert(MinHeap::right_child(+1, -1) == +4);
		assert(MinHeap::right_child(+2, -1) == +6);
		assert(MinHeap::right_child(+3, -1) == +8);

		// Right child, shifted
		assert(MaxHeap::right_child( 0, +4) == -4);
		assert(MaxHeap::right_child(+1, +4) == -2);
		assert(MaxHeap::right_child(+2, +4) ==  0);
		assert(MaxHeap::right_child(+3, +4) == +2);

		// 3-ary heap tests
		// Parent, unshifted
		assert(MinHeap::parent(0, -1, 3) == -1);
		assert(MinHeap::parent(+1, -1, 3) == 0);
		assert(MinHeap::parent(+2, -1, 3) == 0);
		assert(MinHeap::parent(+3, -1, 3) == 0);

		// Parent, shifted
		assert(MaxHeap::parent(0, +4, 3) == +3);
		assert(MaxHeap::parent(+1, +4, 3) == +3);
		assert(MaxHeap::parent(+2, +4, 3) == +3);
		assert(MaxHeap::parent(+3, +4, 3) == +4);


		for (int test_case = 0; test_case < 1000; test_case += 1)
		{
			const long size = 10000;
			std::uniform_real_distribution<double> uniform_double_generator(1.1, 5.9);
			std::uniform_int_distribution<long> uniform_long_generator(0, size-1);
			std::default_random_engine random_number_generator;
			double random_tuple_size = uniform_double_generator(random_number_generator);
			long random_heap_index = uniform_long_generator(random_number_generator);


			long min_left = FractalMinHeap::left_child(random_heap_index, -1L, random_tuple_size);
			long min_right = FractalMinHeap::right_child(random_heap_index, -1L, random_tuple_size);

			long max_left = FractalMaxHeap::left_child(random_heap_index, size, random_tuple_size);
			long max_right = FractalMaxHeap::right_child(random_heap_index, size, random_tuple_size);

			assert(FractalMinHeap::parent(min_left, -1L, random_tuple_size) == random_heap_index);
			assert(FractalMinHeap::parent(min_right, -1L, random_tuple_size) == random_heap_index);
			assert(FractalMaxHeap::parent(max_left, -1L, random_tuple_size) == random_heap_index);
			assert(FractalMaxHeap::parent(max_right, -1L, random_tuple_size) == random_heap_index);
		}
	}
}