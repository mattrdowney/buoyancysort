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
#include "hybrid-hierarchysort.h"
#include "in-place-hierarchysort.h"
#include "insertion-sort.h"
#include "int-that-tracks-comparisons.h"
#include "interlaced-double-binary-heap.h"
#include "interlaced-entangled-double-binary-heap.h"
#include "max-heap.h"
#include "median-of-medians.h"
#include "min-heap.h"
#include "ordered-max-heap.h"
#include "ordered-min-heap.h"
#include "out-of-place-hierarchysort.h"
#include "print.h"
#include "quicksort.h"
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
	const long size = 1000000;
	const long tuple_size = 2;
	std::vector<current_type> data(size);
	for (std::size_t i = 0; i < size; i += 1)
	{
		/*if ((i % 2) == 0)
		{
			data[i] = size/2;
			continue;
		}*/
		data[i] = size - i;
	}
	
	std::random_device random_device;
	std::mt19937 random_number_generator(random_device());
	random_number_generator.seed(26);
	std::shuffle(&data[0], (&data[size-1]) + 1, random_number_generator);
	
	//Print::print((current_type*)data.data(), -1, size);
	
	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);

	//IntThatTracksComparisons::reset_comparisons();

	//Hierarchysort::in_place<current_type>((current_type*)data.data(), -1, size);
	std::vector<long> gap_sequence1 = ShellSort::extrapolated_ciura_tokuda4;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_ciura4, 25);
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda5;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	//ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 45000, 120000, 40991, 41200, 41);
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 150, 400, 10000);
	//ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 50000, 100000, 41286, 41342-20, 41342+20, 11);

	// 1, 4, 10, 23, 55, 143, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 848473387:883022930 (34549543) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 40000, 100000, 500);
	// Extra comparisons: ((34549543/500 trials)/((40000+100000)/2)) average or ~0.9871298; I'm thinking that may not be a coincidence (and it's related to "analytical overloading" as I call it)
	// One idea: before starting the ShellSort, always do something similar to Median-Of-Medians.
	// It's hard to figure out how far to take this, but since this Catalan convolution sequence seems to be far greater than any finite multiplier you need something to ensure it works for large arrays.
	// The original idea was to compare groups of five, so you would at least have some reliable sorting at the beginning (because you can get really unlucky asymptotically).
	// I think there's probably a better idea, perhaps one that no longer uses ShellSort.
	// Also, one thing I've been pretty curious about for a while: what is the constant factor of the best ShellSort? E.g. is it similar to Apéry's constant -- "The reciprocal of zeta(3) is the probability that any three positive integers, chosen at random, will be relatively prime" - https://en.wikipedia.org/wiki/Ap%C3%A9ry%27s_constant

	// It's also good to test up to 143 (not the full sequence I've developed):
	// 1, 4, 10, 23, 55, 143, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 27236421:27268759 (32338) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 150, 400, 10000);

	// Why? Because you find better results:
	// 1, 4, 10, 23, 55, 144, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 27262352:27239770 (-22582) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 150, 400, 10000);
	// 1, 4, 10, 23, 57, 143, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 27270066:27177591 (-92475) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 150, 400, 10000);
	// 1, 4, 10, 23, 57, 145, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 27254963:27145624 (-109339)
	
	// Recalibrating around new sequence: (benchmark changed)
	// (None of the other values seem to work well, although my tests are pretty basic)

	//long long comparisons = IntThatTracksComparisons::get_comparisons();

	//Sorted::verify((current_type*)data.data(), -1, size);
	//Print::print((current_type*)data.data(), -1, size);

	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	
	//std::cout << comparisons << std::endl;
	//float cost_per_element = ((float)comparisons) / size;
	//std::cout << cost_per_element << std::endl;

	char word;
	std::cin >> word;
	return 0;
}