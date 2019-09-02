#include "stdafx.h"
#include <algorithm>
#include <functional>
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
	const long size = 1000;
	const long tuple_size = 2;
	std::vector<current_type> data(size); // I really should have used a container with unbounded size sooner (stack size is limited)
	// the cool thing about the IntThatTracksComparisons function is it can work with std::partition, TimSort, etc (even if it has a blackbox implementation) -- plus it's easier to implement
	for (std::size_t i = 0; i < size; i += 1)
	{
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(1);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();
	
	std::vector<long> result = ShellSort::generalized_pratt(std::set<long>{ 2, 3, 5, 7, 11 }, 25);
	double a = 1;
	double b = 4 - 1.61803398874989484820; // 4 minus phi (golden ratio)
	std::function<long(long)> lambda_function = [a, b](long n)
	{
		double unrounded = (pow(b, n) - 1) / (b - 1);
		long rounded = (n % 2) == 0 ? floor(unrounded) : ceil(unrounded);
		return rounded;
	};

	std::function<long(long)> lambda_function2 = [a, b](long n)
	{
		double value = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value = b * value + pow(1 + 1.0/535.491656, n-1); // (1-(2*pi/n))^n, n=2000 // using the equation for euler's number except 1) subtracting and 2) using 2*pi instead of 1 // aka e^-(2*pi) apparently
		}
		return ceil(value);
	};

	result = ShellSort::gap_sequence_generator<long>(lambda_function2, 20L);
	for (std::vector<long>::const_iterator iterator = result.begin(); iterator != result.end(); ++iterator)
	{
		std::cout << *iterator << ' ';
	}
	std::cout << std::endl;

	//ShellSort::sort((current_type*)data.data(), -1, size, result);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::tokuda_gap_sequence);
	ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::ciura_gap_sequence);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::trying_to_beat_ciura);

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