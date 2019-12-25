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
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda10;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	//ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 45000, 120000, 40991, 41200, 41);
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 25);
	//ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 150, 400, 145, 145+1, 145+20, 5000);

	// 1, 4, 10, 23, 57, 146, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13601105:13608420 (7315) // ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 150, 400, 145, 145 + 1, 145 + 20, 5000);
	//1, 4, 10, 23, 57, 147, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626111:13667508 (41397)
	//1, 4, 10, 23, 57, 148, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626278 : 13761762 (135484)
	//1, 4, 10, 23, 57, 149, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626111 : 13679402 (53291)
	//1, 4, 10, 23, 57, 150, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13624414 : 13660681 (36267)
	//1, 4, 10, 23, 57, 151, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13590171 : 13611020 (20849)
	//1, 4, 10, 23, 57, 152, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626278 : 13630653 (4375)
	//1, 4, 10, 23, 57, 153, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13623850 : 13623155 (-695)
	//1, 4, 10, 23, 57, 154, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13597351 : 13593430 (-3921)
	//1, 4, 10, 23, 57, 155, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626136 : 13624467 (-1669)
	//1, 4, 10, 23, 57, 156, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13576122 : 13572071 (-4051)
	//1, 4, 10, 23, 57, 157, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13584026 : 13589565 (5539)
	//1, 4, 10, 23, 57, 158, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13599952 : 13605690 (5738)
	//1, 4, 10, 23, 57, 159, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13623850 : 13676820 (52970)
	//1, 4, 10, 23, 57, 160, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13584186 : 13669351 (85165)
	//1, 4, 10, 23, 57, 161, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13626278 : 13657984 (31706)
	//1, 4, 10, 23, 57, 162, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13582744 : 13586943 (4199)
	//1, 4, 10, 23, 57, 163, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13624390 : 13638042 (13652)
	//1, 4, 10, 23, 57, 164, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13601161 : 13608574 (7413)
	//1, 4, 10, 23, 57, 165, 405, 1228, 3910, 12884, 43516, 149730, 522742, 1846666, 6587930, 23698479, 85861543, 313027067, 1147476081, 13616785 : 13608033 (-8752)

	// I'm gonna sort of ignore the higher order ones (I think they might be dead ends), but this sequence might be it:
	// http://oeis.org/A226845
	// 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083, 105185, 269297, 689465, 1765209, 4519393, 11570803, 29624209, 75845551, 194184025, 497160801, 1272858861, 3258844373, 8343475473, 21361432171, 54690732373, 140022269235
	// If you check, the sequence seems to asymptotically approach ~2.56 (which is approximately that .5*(2.4+e) ratio
	// Otherwise, it seems to be in the same ballpark as my current gap sequence.
	// 1, 4, 10, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083, 42812037:44632536 (1820499) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 25);
	// 1, 4, 10, 23, 57, 145, 361, 925, 2383, 6150, 15905, 41326, 43301770:43621394 (319624)
	// both have no improvement, but I think I'm on the right track with http://oeis.org/A226845
	// Why? If you look, the numbers seem to be nearby:
	// {1, 3, 9, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083 } - { 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15905, 41326 } =>
	// { 0, -1, -1, 0, 0, 3, 12, 32, 66, 117, 140, -243 }
	// try: {1, 3, 9, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083 } - { 0, -1, -1, 0, 0, 3, 12, 27, 48, 75, 108, 147 }
	// thus: {1, 4, 10, 23, 57, 142, 361, 930, 2401, 6192, 15937, 40936}
	// which had no luck:
	// 1, 4, 10, 23, 57, 142, 361, 930, 2401, 6192, 15937, 40936, 47611731:47882481 (270750) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 25);
	// Instead try: {1, 3, 9, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083 } + {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0 }
	// or: { 1, 4, 10, 23, 57, 145, 374, 958, 2450, 6268, 16045, 41083 }
	// which also had no luck:
	// 1, 4, 10, 23, 57, 145, 374, 958, 2450, 6268, 16045, 41083, 43614800:44271067 (656267) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 25);
	// I might do the following: make this a benchmark and try to improve it by creating fluctuations around the center
	// Approximately:
	// {1, 3, 9, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083 } + {0, 1, 1, 0, -2, -2, 0, +3, +3, 0, -4, -4 }
	// or: { 1, 4, 10, 23, 55, 143, 373, 960, 2452, 6267, 16041, 41079 }
	// Uhgh, no luck:
	// 1, 4, 10, 23, 55, 143, 373, 960, 2452, 6267, 16041, 41079, 44240602:45687451 (1446849)
	// Maybe:
	// { 1, 3, 9, 23, 57, 145, 373, 957, 2449, 6267, 16045, 41083 } + {0, 1, 1, 0, 0, 0, -12, -12, -12, -12, 0, 0 }
	// or: { 1, 4, 10, 23, 57, 145, 361, 945, 2437, 6255, 16045, 41083 }
	// Worse than some of the previous attempts, but now that I think about it these attempts are okay (they're close enough to striking distance).
	// 1, 4, 10, 23, 57, 145, 361, 945, 2437, 6255, 16045, 41083, 42978124:43280474 (302350)

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