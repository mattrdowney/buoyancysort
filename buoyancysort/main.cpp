#include "stdafx.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <set>
#include <utility>
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
#include "shell-sort.h"
#include "sift.h"
#include "sorted.h"
#include "spindle.h"
#include "stable-partition.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	//HeapTests::heap_tests();
	const long size = 1000;
	const long tuple_size = 2;
	std::vector<current_type> data(size);
	for (std::size_t i = 0; i < size; i += 1)
	{
		if ((i % 2) == 0)
		{
			data[i] = 50;
			continue;
		}
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(20);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();

	//std::vector<long> ciura_approximation = ShellSort::gap_sequence_generator(ShellSort::ciura_approximation3, 25);
	std::vector<long> pratt = ShellSort::generalized_pratt(std::set<long>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97}, 9409, false);
	for (long gap : pratt)
	{
		std::cout << gap << ", ";
	}
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::ciura_extended_gap_sequence_attempt8);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::probably_reliable);
	ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::ciura_gap_sequence);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::tokuda_gap_sequence);

	//StablePartition::three_way((current_type*)data.data(), -1, size, size/2);

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