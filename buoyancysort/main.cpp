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
#include "ordered-max-heap.h"
#include "ordered-min-heap.h"
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
	
	Print::print((current_type*)data, -1, size);
	
	HeapTests::test_min_heap<current_type>((current_type*)data, -1, size, 5);
	//HeapTests::test_max_heap<current_type>((current_type*)data, -1, size, 5);
	HeapTests::test_min_heap_alignment<current_type>((current_type*)data, -1, size, 5);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data, -1, size, 5);

	IntThatTracksComparisons::reset_comparisons();

	OrderedMinHeap::build<current_type>((current_type*)data, -1, size, 5);

	std::size_t comparisons = IntThatTracksComparisons::get_comparisons();

	Print::print((current_type*)data, -1, size);

	HeapTests::test_min_heap<current_type>((current_type*)data, -1, size, 5);
	//HeapTests::test_max_heap<current_type>((current_type*)data, -1, size, 5);
	HeapTests::test_min_heap_alignment<current_type>((current_type*)data, -1, size, 5);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data, -1, size, 5);
	
	std::cout << comparisons << std::endl;
	float cost_per_element = ((float)comparisons) / size;
	std::cout << cost_per_element << std::endl;

	char word;
	std::cin >> word;
	return 0;
}