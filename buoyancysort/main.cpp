#include "stdafx.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <math.h>
#include <random>
#include <set>
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
	
	//std::random_device random_device;
	//std::mt19937 random_number_generator(random_device());
	//random_number_generator.seed(13);
	//std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	IntThatTracksComparisons::reset_comparisons();

	const double e = 2.7182818284590452353602874713527;
	const double phi = 1.61803398874989484820;
	const double pi = 3.14159265358979323846;

	std::vector<long> result = ShellSort::generalized_pratt(std::set<long>{ 2, 3 }, 25);
	double a = 1;
	double i = 67.0;
	double j = 103.0;
	double b = 4 - 1.61803398874989484820; // 4 minus phi (golden ratio)
	double b2 = 9.0 / 4;
	double c = 1 + 1.0 / 535.491656; //1 + e^(-2*pi) // similar to euler's formula / identity when x=i*pi

	std::function<long(long)> ciura_approximation = [a, b, b2, c](long n)
	{
		double value1 = 1;
		double value2 = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value1 = b * value1 + 1;
		}
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value2 = b2 * value2 + 1;
		}
		double r = 0.99101670897; // 1 - 1/(e^(2*(pi*3/4)))
		return ceil(pow(value1, r)*pow(value2, 1-r));
	};

	std::function<long(long)> ciura_approximation2 = [phi, e, pi](long n)
	{
		const double center = 2.41;
		//const double radius = 1 / pow(e, 1.5*pi);
		//const double left = center - radius;
		//const double right = center + radius;
		//const double radius = 1 / pow(e, 1.25*pi);
		//const double left = center / (1+radius);
		//const double right = center * (1+radius);
		const double min_gap = 2.2878; // empirically determined
		const double max_gap = 2.48;//2.474; // empirically determined by https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence
		//const double min_gap = 2.25;
		//const double max_gap = pow(center / sqrt(min_gap), 2);
		double value = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			if ((iteration % 2) == 1) // odd
			{
				value = min_gap * value + 1;
			}
			else // even
			{
				value = max_gap * value + 1;
			}
		}
		std::cout << value << std::endl;
		return ceil(value);
	};

	//result = ShellSort::gap_sequence_generator<long>(ciura_approximation2, 20L);
	//result = ShellSort::gap_sequence_generator<long>([phi, e, pi](long n) { return pow(2.282/*(4 - phi) + 1 / pow(e, .75*pi)*/, n - 1); }, 20L);
	for (std::vector<long>::const_iterator iterator = result.begin(); iterator != result.end(); ++iterator)
	{
		std::cout << *iterator << ' ';
	}
	std::cout << std::endl;

	//ShellSort::sort((current_type*)data.data(), -1, size, result);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::tokuda_gap_sequence);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::ciura_gap_sequence);
	//ShellSort::sort((current_type*)data.data(), -1, size, ShellSort::pratt_technique7);
	CombSort::not_combsort((current_type*)data.data(), -1, size, result);

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