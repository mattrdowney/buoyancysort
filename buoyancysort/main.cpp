#include "stdafx.h"
#include <iostream>
#include "buoyancysort.h"
#include "bubble-sort.h"
#include "insertion-sort.h"
#include "heap.h"
#include "heap-tests.h"
#include "hoare-partition.h"
#include "median-of-medians.h"

int main()
{
	HeapTests::heap_tests();
	const int size = 64;
	int data[size];
	for (int i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}

	for (int datum : data)
	{
		std::cout << datum << " ";
	}

	std::cout << std::endl;

	Heap::build_min_heap<int>((int*)data, 0, (std::size_t)(sizeof(data) / sizeof(data[0])));
	for (int datum : data)
	{
		std::cout << datum << " ";
	}
	char word;
	std::cin >> word;
	return 0;
}