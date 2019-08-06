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
#include "print.h"

int main()
{
	HeapTests::heap_tests();
	const int size = 10000;
	int data[size]; // TODO: define IntThatTracksComparisons that overrides definitions of < >, <=, >=, ==, != and increments a static counter when they are called
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (int i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}

	Print::print((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	/*
	HeapTests::test_min_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	InterlacedDoubleBinaryHeap::build<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	Print::print((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	*/

	HeapTests::test_min_heap<int>((int*)data, -1 + 10, (std::size_t)(sizeof(data) / sizeof(data[0])) - 10);
	HeapTests::test_max_heap<int>((int*)data, -1 + 10, (std::size_t)(sizeof(data) / sizeof(data[0])) - 10);

	InterlacedDoubleBinaryHeap::build<int>((int*)data, -1 + 10, (std::size_t)(sizeof(data) / sizeof(data[0])) - 10);

	Print::print((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<int>((int*)data, -1 + 10, (std::size_t)(sizeof(data) / sizeof(data[0])) - 10);
	HeapTests::test_max_heap<int>((int*)data, -1 + 10, (std::size_t)(sizeof(data) / sizeof(data[0])) - 10);

	char word;
	std::cin >> word;
	return 0;
}