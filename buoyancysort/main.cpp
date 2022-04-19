#include "stdafx.h"
#include <algorithm>
#include <ctime>
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
	const long size = 10000000;
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

	IntThatTracksComparisons::reset_comparisons();

	std:clock_t start = std::clock();
	//std::sort(data.begin(), data.end());
	//std::stable_sort(data.begin(), data.end());
	//Hierarchysort::out_of_place(data.data(), -1, size);

	// std::sort (control, 10,000,000, Release): 1.344 seconds 
	// std::stable_sort (control, 10,000,000, Release): 1.175 seconds (surprisingly)
	// Unoptimized hierarchysort (original, null hypothesis, 10,000,000, Release): 1.765 seconds 

	//Hierarchysort::in_place<current_type>((current_type*)data.data(), -1, size);
	std::vector<long> gap_sequence1 = ShellSort::extrapolated_ciura_tokuda1;
	//std::vector<long> gap_sequence1 = ShellSort::gap_sequence_generator(ShellSort::generalized_invisal_best, 20);
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::modified_tokuda, 25);
	//gap_sequence2.insert(gap_sequence2.begin(), 1);
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_invisal_best, 20); // this isn't terrible, it seems to 
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_ciura4, 25);
	//std::vector<long> gap_sequence2 = ShellSort::old_modified_tokuda;
	//std::vector<long> gap_sequence2 = ShellSort::modified_tokuda2; // this version with primes does worse // (12,713,270) versus (7,163,058) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 2, 1000000, 41); where gap_sequence2 is the unrounded up to prime version
	//std::vector<long> gap_sequence2 = ShellSort::generating_function_roots;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda_retry;
	//ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 1820, 3600, 1700, 1800, 1000);
	ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence2, 1800, 6000, 1173, 700, 1500, 3000);
	//ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 3900, 9750, 1557, 1557-50, 1557+50, 3000);

		//1, 4, 10, 23, 57, 145, 364, 915, 2990, 6165, 15916, 41332, 194838170:194760859 (-77311)
		//1, 4, 10, 23, 57, 145, 364, 916, 2990, 6165, 15916, 41332, 194488386 : 195613228 (1124842)
		//1, 4, 10, 23, 57, 145, 364, 917, 2990, 6165, 15916, 41332, 194403771 : 193959115 (-444656)
		//1, 4, 10, 23, 57, 145, 364, 918, 2990, 6165, 15916, 41332, 194894104 : 195553375 (659271)
		//1, 4, 10, 23, 57, 145, 364, 919, 2990, 6165, 15916, 41332, 194676036 : 194856926 (180890)
		//1, 4, 10, 23, 57, 145, 364, 920, 2990, 6165, 15916, 41332, 195394584 : 194812343 (-582241) // 920 is really good, 1207 seemed to be really off the mark.
		//1, 4, 10, 23, 57, 145, 364, 921, 2990, 6165, 15916, 41332, 195229384 : 196273601 (1044217)
		//1, 4, 10, 23, 57, 145, 364, 922, 2990, 6165, 15916, 41332, 193486598 : 193427379 (-59219)
		//1, 4, 10, 23, 57, 145, 364, 923, 2990, 6165, 15916, 41332, 195678308 : 195506448 (-171860)
		//1, 4, 10, 23, 57, 145, 364, 924, 2990, 6165, 15916, 41332, 196412303 : 196766018 (353715)
		//1, 4, 10, 23, 57, 145, 364, 925, 2990, 6165, 15916, 41332, 197066695 : 197115820 (49125)

	double run_time = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	long long comparisons = IntThatTracksComparisons::get_comparisons();

	//Sorted::verify((current_type*)data.data(), -1, size);
	//Print::print((current_type*)data.data(), -1, size);

	//HeapTests::test_min_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_min_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	//HeapTests::test_max_heap_alignment<current_type>((current_type*)data.data(), -1, size, tuple_size);
	
	std::cout << comparisons << std::endl;
	float cost_per_element = ((float)comparisons) / size;
	std::cout << cost_per_element << std::endl;
	//std::cout << run_time << std::endl;

	char word;
	std::cin >> word;
	return 0;
}