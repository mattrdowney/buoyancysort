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
	//ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 45000, 120000, 300);
	ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence2, 45000, 120000, 41286, 41100, 41102, 11);

	// Failure: 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41072, 603408529:603515304 (106775) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 45000, 120000, 300);

	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41111, 21971051:21985160 (14109) // ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence2, 45000, 120000, 41286, 41043, 41317, 11);
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41112, 22294986:22288076 (-6910) // I fucked up, I only captured the most recent values, I was intending to investigate 41100-41102 (I decided to stop early because I figured the multiplier won't be that close to 2.6 yet (ignoring the fact that my sequence could be on the wrong trajectory, which is likely but I have to start somewhere).
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41113, 23180605 : 23209343 (28738)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41114, 20822167 : 20862370 (40203)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41115, 26445002 : 26436954 (-8048)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41116, 25075492 : 25060616 (-14876)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41117, 23884422 : 23849706 (-34716)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41118, 22232845 : 22210456 (-22389)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41119, 23299648 : 23315429 (15781)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41120, 25522124 : 25546947 (24823)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41121, 26037605 : 26029179 (-8426)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41122, 20395753 : 20416597 (20844)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41123, 23636539 : 23645921 (9382)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41124, 24549519 : 24547303 (-2216)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41125, 23203142 : 23226166 (23024)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41126, 20776734 : 20805734 (29000)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41127, 19638200 : 19629781 (-8419)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41128, 25081231 : 25110929 (29698)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41129, 20825026 : 20849824 (24798)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41130, 22293913 : 22336968 (43055)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41131, 21688260 : 21721922 (33662)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41132, 22701096 : 22714418 (13322)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41133, 23762161 : 23766139 (3978)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41134, 24497097 : 24501952 (4855)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41135, 24224111 : 24218256 (-5855)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41136, 23431738 : 23433855 (2117)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41137, 23173396 : 23146812 (-26584)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41138, 24571371 : 24611416 (40045)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41139, 20739360 : 20776447 (37087)

	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41100, 638777946 : 638860136 (82190) // ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence2, 45000, 120000, 41286, 41100, 41102, 300);
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41101, 620815002 : 621057655 (242653)
	//1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15891, 41102, 631609619 : 631844158 (234539)

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