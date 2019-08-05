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
	int data[size]; // TODO: define IntThatTracksComparisons that overrides definitions of < >, <=, >=, and == and increments a static counter when they are called
	for (int i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}

	Print::print((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	Buoyancysort::sort<int>((int*)data, (std::size_t)(sizeof(data) / sizeof(data[0])));

	Print::print((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	HeapTests::test_min_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));
	HeapTests::test_max_heap<int>((int*)data, -1, (std::size_t)(sizeof(data) / sizeof(data[0])));

	char word;
	std::cin >> word;
	return 0;
}