#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include "bubble-sort.h"
#include "buoyancysort.h"
#include "heap-tests.h"
#include "hoare-partition.h"
#include "insertion-sort.h"
#include "int-that-tracks-comparisons.h"
#include "interlaced-double-binary-heap.h"
#include "interlaced-entangled-double-binary-heap.h"
#include "max-heap.h"
#include "median-of-medians.h"
#include "min-heap.h"
#include "print.h"
#include "reverse.h"
#include "shell-sort.h"
#include "sift.h"
#include "spindle.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	HeapTests::heap_tests();
	const std::size_t size = 1000;
	current_type data[size]; // TODO: define IntThatTracksComparisons that overrides definitions of < >, <=, >=, ==, != and increments a static counter when they are called
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (std::size_t i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(0);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	
	//HeapTests::test_min_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_max_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));

	IntThatTracksComparisons::reset_comparisons();

	//ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::ciura_gap_sequence);
	//ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::root_five_silver_integer_with_memory_gap_sequence);
	//ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::pratt_three_smooth_gap_sequence);
	//ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::tokuda_gap_sequence);
	//ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::intuition_based_testing_gap_sequence3);
	ShellSort::sort<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])), ShellSort::two_and_a_quarter_silver_integer_with_memory_gap_sequence);
	
	// This is why we test against good functions that have infinite range (e.g. Tokuda). Apparently Tokuda's 2.25 is better than sqrt(5) [I was really wrong, as usual]
	
	std::size_t comparisons = IntThatTracksComparisons::get_comparisons();

	//Print::print((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));

	//HeapTests::test_min_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_max_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	
	std::cout << comparisons << std::endl;
	float cost_per_element = ((float)comparisons) / size;
	float log2_N = std::log2(size);
	std::cout << (std::log2(cost_per_element)/std::log2(log2_N)) << std::endl;

	char word;
	std::cin >> word;
	return 0;
}