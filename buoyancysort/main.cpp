#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include <set>
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
#include "semi-stable-partition.h"
#include "shell-sort.h"
#include "sift.h"
#include "spindle.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

int main()
{
	//HeapTests::heap_tests();
	const long size = 1000000;
	const long tuple_size = 2;
	std::vector<current_type> data(size); // I really should have used a container with unbounded size sooner (stack size is limited)
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (std::size_t i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(2);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();
	
	// (Ignoring the fatal bug) this worked pretty well for a first attempt.
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::hybridized_pratt_3_5_squared);
	std::vector<long> result = ShellSort::generalized_pratt(std::set<long>{ 2, 3, 5, 7 }, 20);
	for (std::vector<long>::const_iterator iterator = result.begin(); iterator != result.end(); ++iterator)
	{
		std::cout << *iterator << ' ';
	}
	// Eventually, I'll want to generalize this even more.
	// For instance: up until now I've used a hybrid approach (which is definitely possible with two layers),
	// That being said, you might be able to do 3+ layers of sorting:
	// E.g. first use 3-smooth squares, then 5-smooth cubes, then 7-smooth quads, etc.
	// If I had to guess, you would transition at factorials or something to guarantee smoothness.
	// This does seem like a good idea and relatively easy to test (albeit hardcoded).
	// The only thing I don't know how to account for: the seams between segments.
	// I'm pretty sure I want to use 2-smooth numbers up until 2!, 3-smooth squares up until 5!, 5-smooth cubes up until 7!, ...
	// What I can't know until I test is whether or not you need to add an extra value to help with interpolation
	// (It's already a stretch that this works, at least on the first try.)
	ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::generalized_pratt(std::set<long>(result.begin(), result.end()), 50)); // Better performance yet

	//SemiStablePartition::partition<current_type>((current_type*)data.data(), -1, size, size/2);

	std::size_t comparisons = IntThatTracksComparisons::get_comparisons();

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