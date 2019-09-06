#include "stdafx.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <set>
#include "bubble-sort.h"
#include "buoyancysort.h"
#include "comb-sort.h"
#include "heap-tests.h"
#include "hoare-partition.h"
#include "insertion-sort.h"
#include "int-that-tracks-comparisons.h"
#include "interlaced-double-binary-heap.h"
#include "interlaced-entangled-double-binary-heap.h"
#include "max-heap.h"
#include "median-of-medians.h"
#include "min-heap.h"
#include "ordered-max-heap.h"
#include "ordered-min-heap.h"
#include "print.h"
#include "reverse.h"
#include "semi-stable-partition.h"
#include "stable-three-way-partition.h"
#include "shell-sort.h"
#include "sift.h"
#include "sorted.h"
#include "spindle.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	//HeapTests::heap_tests();
	const long size = 1000000;
	const long tuple_size = 2;
	std::vector<current_type> data(size);
	for (std::size_t i = 0; i < size; i += 1)
	{
		if ((i % 2) == 0)
		{
			//data[i] = 50;
			//continue;
		}
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(13);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();

	std::vector<long> triangular = ShellSort::gap_sequence_generator([](long n)
	{
		// http://oeis.org/A081659
		std::map<long, long> fibonacci_thing
		{
				{1, 1}, {2, 2}, {3, 4}, {4, 6}, {5, 9},
				{6, 13}, {7, 19}, {8, 28}, {9, 42}, {10, 64},
				{11, 99}, {12, 155}, {13, 245}, {14, 390}, {15, 624},
				{16, 1002}, {17, 1613}, {18, 2601}, {19, 4199}, {20, 6784},
				{21, 10966}, {22, 17732}, {23, 28679}, {24, 46391}, {25, 75049}
				//121418, 196444, 317838, 514257, 832069, 1346299, 2178340, 3524610, 5702920, 9227499, 14930387, 24157853, 39088206 1, 2, 4, 6, 9, 13, 19, 28, 42, 64, 99, 155, 245, 390, 624, 1002, 1613, 2601, 4199, 6784, 10966, 17732, 28679, 46391, 75049, 121418, 196444, 317838, 514257, 832069, 1346299, 2178340, 3524610, 5702920, 9227499, 14930387, 24157853, 39088206 }
		};
		long n2 = ceil(pow(1.55, n - 1)); //fibonacci_thing[n];
		return n2 * (n2 + 1) / 2;
	}, 23);
	for (long value : triangular)
	{
		std::cout << value << ", ";
	}

	ShellSort::sort((current_type*)data.data(), -1, size, triangular);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::tokuda_gap_sequence);
	//StableThreeWayPartition::partition((current_type*)data.data(), -1, size, size/2);

	std::size_t comparisons = IntThatTracksComparisons::get_comparisons();

	Sorted::verify((current_type*)data.data(), -1, size);
	//Print::print((current_type*)data.data(), -1, size);

	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	
	std::cout << comparisons << std::endl;
	float cost_per_element = ((float)comparisons) / size;
	std::cout << cost_per_element << std::endl;

	char word;
	std::cin >> word;
	return 0;
}