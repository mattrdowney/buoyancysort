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
	random_number_generator.seed(1);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();
	
	// (Ignoring the fatal bug) this worked pretty well for a first attempt.
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::hybridized_pratt_3_5_squared);
	std::vector<long> result = ShellSort::generalized_pratt(std::set<long>{ 2, 3, 5, 7, 11 }, 25);
	double a = 1;
	double b = 4 - 1.61803398874989484820;
	std::function<long(long)> lambda_function = [a, b](long n)
	{
		double unrounded = (pow(b, n) - 1) / (b - 1);
		long rounded = (n % 2) == 0 ? floor(unrounded) : ceil(unrounded);
		return rounded;
	};
	// So a few comments related to this function:
	// 1) I was probably overthinking it when I said you need to find the right underestimate numbers that don't repeat redundant actions.
	// 2) An easy way of thinking about this problem is threading a needle around an asymptotic bound.
	//        Essentially what you want to do is alternate in some fasion above and below the equation (preferably with prime numbers).
	//        At first I was leaning towards doing 1 floor_to_prime ceil_to_prime floor ceil floor, because it would give better ratios for the first few numbers
	//        But then I thought about it philosophically and realized that would mean 1 should be ceiled_to_prime (so 2) which would be incorrect
	//        So I decided the alternating (at least when it alternates on even/odd and not according to some other math) should be:
	//        1 5 7 23 53 131 313 751 1783...
	// 3) Why do you want to thread a needle around an asymptotic bound?
	//        My theory would be sort of related to the primes. If you have a mathematical boundary and alternate across it, then you force
	//        Prime numbers to follow a particular pattern.
	//        You could think about it as using guaranteed or semi-guaranteed statistics to sort.
	//        I'd hope it's easy to do a worst case analysis, but maybe it's harder than I think (especially since few gap sequences have worst case analysis to back them up).
	// Now to see if it works.

	double c = 1.0073;
	std::function<long(long)> lambda_function2 = [a, b, c](long n)
	{
		double value = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value = b * value + c;
		}
		return ceil(value);
	};
	

	result = ShellSort::gap_sequence_generator<long>(lambda_function2, 20L);
	for (std::vector<long>::const_iterator iterator = result.begin(); iterator != result.end(); ++iterator)
	{
		std::cout << *iterator << ' ';
	}
	std::cout << std::endl;
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
	ShellSort::sort((current_type*)data.data(), -1, size, result); // Eh, I guess it didn't work.
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::tokuda_gap_sequence);
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