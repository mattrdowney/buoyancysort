#include "stdafx.h"
#include <algorithm>
#include <iostream>
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
#include "sift.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	HeapTests::heap_tests();
	const std::size_t size = 101;
	current_type data[size]; // TODO: define IntThatTracksComparisons that overrides definitions of < >, <=, >=, ==, != and increments a static counter when they are called
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (std::size_t i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}

	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);

	Print::print((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	
	HeapTests::test_min_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));

	IntThatTracksComparisons::reset_comparisons();

	Sift::leftward_sift<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));

	std::size_t comparisons = IntThatTracksComparisons::get_comparisons();

	Print::print((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<current_type>((current_type*)data, -1, (sizeof(data) / sizeof(data[0])));
	
	std::cout << comparisons << std::endl;

	char word;
	std::cin >> word;
	return 0;
}