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
	std::vector<long> gap_sequence1 = ShellSort::ciura_gap_sequence;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_ciura4, 25);
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda4;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	for (long gap : gap_sequence2)
	{
		std::cout << gap << ", ";
	}
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 300, 1500, 10000);
	//ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 100000, 1000000);
	
	// 1, 4, 10, 23, 57, 132, 331, 837, 115287637:115068185 (-219452)
	// 1, 4, 10, 23, 57, 142, 361, 910, 115268196:114914994 (-353202)
	// 1, 4, 10, 23, 57, 142, 361, 908, 114922836:114560936 (-361900)
	// 1, 4, 10, 23, 57, 142, 361, 825, 115166001:114920061 (-245940)
	// 1, 4, 10, 23, 57, 142, 361, 854, 115285768:114967249 (-318519)
	// 1, 4, 10, 23, 57, 142, 356, 892, 115264957:114893134 (-371823)

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