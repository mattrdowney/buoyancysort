#include "stdafx.h"
#include <iostream>
#include "buoyancysort.h"
#include "bubble-sort.h"
#include "insertion-sort.h"
#include "min-heap.h"
#include "max-heap.h"
#include "heap-tests.h"
#include "hoare-partition.h"
#include "median-of-medians.h"
#include "interlaced-double-binary-heap.h"
#include "interlaced-entangled-double-binary-heap.h"
#include "print.h"
#include "int-that-tracks-comparisons.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	HeapTests::heap_tests();
	const std::size_t size = 64;
	current_type data[size]; // TODO: define IntThatTracksComparisons that overrides definitions of < >, <=, >=, ==, != and increments a static counter when they are called
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (std::size_t i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}

	std::cout << IntThatTracksComparisons::get_comparisons() << std::endl;

	Print::print((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	
	HeapTests::test_min_heap<current_type>((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<current_type>((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	InterlacedEntangledDoubleBinaryHeap::build<current_type>((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	Print::print((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<current_type>((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<current_type>((current_type*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	
	std::cout << IntThatTracksComparisons::get_comparisons() << std::endl;

	char word;
	std::cin >> word;
	return 0;
}