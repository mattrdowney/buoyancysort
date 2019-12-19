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
	std::vector<long> gap_sequence1 = ShellSort::extrapolated_ciura_tokuda1;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_ciura4, 25);
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda2;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	//ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 45000, 120000, 40991, 41200, 41);
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 2, 100000, 500);
	//ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 50000, 100000, 41286, 41342-20, 41342+20, 11);

	// You can use https://alteredqualia.com/visualization/hn/sequence/
	// to make general inferences about where the next gap sequence value is going to be, which is how I decided on 41342
	// as my next point of exploration.

	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41322, 20204434:20222035 (17601) // ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 50000, 100000, 41286, 41342-20, 41342+20, 11);
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41323, 20662006:20685007 (23001)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41324, 18734374 : 18733273 (-1101)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41325, 20009930 : 20017795 (7865)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41326, 23196554 : 23156939 (-39615)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41327, 18641016 : 18650067 (9051)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41328, 20442344 : 20443351 (1007)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41329, 20173852 : 20161905 (-11947)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41330, 19724892 : 19737836 (12944)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41331, 19963036 : 19968785 (5749)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41332, 20453997 : 20461898 (7901)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41333, 18450091 : 18456360 (6269)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41334, 21636476 : 21641013 (4537)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41335, 20469063 : 20513571 (44508)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41336, 19143846 : 19146626 (2780)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41337, 20475208 : 20487915 (12707)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41338, 21967487 : 21987583 (20096)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41339, 21201891 : 21203964 (2073)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41340, 20151703 : 20192165 (40462)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41341, 19988094 : 20014397 (26303)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41342, 20635884 : 20697278 (61394)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41343, 22775554 : 22783104 (7550)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41344, 21103942 : 21112694 (8752)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41345, 20682005 : 20703858 (21853)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41346, 22363735 : 22386174 (22439)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41347, 21747751 : 21795844 (48093)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41348, 18656937 : 18651226 (-5711)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41349, 19530812 : 19497974 (-32838)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41350, 19784568 : 19813264 (28696)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41351, 22044884 : 22044507 (-377)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41352, 21683023 : 21679606 (-3417)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41353, 21994263 : 21989252 (-5011)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41354, 19156187 : 19155177 (-1010)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41355, 19883968 : 19888238 (4270)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41356, 21214789 : 21223758 (8969)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41357, 20013407 : 20021044 (7637)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41358, 22340607 : 22332156 (-8451)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41359, 20044462 : 20058149 (13687)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41360, 20484947 : 20497856 (12909)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41361, 20477424 : 20498258 (20834)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41362, 20747865 : 20745041 (-2824)

	// Using 41326 yields -24 for the next discrete difference, which is accelerating faster than expected but about right.
	// Usually if I see two values that seem right, I take the lower one on the range (this sort of assumes a mostly bimodal distribution (with other local maxima that are not quite as good)).

	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41326, 585189113:585114946 (-74167) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 2, 100000, 500);

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