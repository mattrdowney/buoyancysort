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
	std::vector<long> gap_sequence1 = ShellSort::tokuda_gap_sequence;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::generalized_ciura4, 25);
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda4;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	for (long gap : gap_sequence2)
	{
		std::cout << gap << ", ";
	}
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 6200, 15800, 4000);
	//ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 100000, 1000000);

	// Versus Tokuda:
	// 1, 4, 10, 23, 57, 142, 361, 925, 2385, 6160, 15924, 41176, 106493, 275442, 712445, 77508576:77021820 (-486756) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 3000, 6000, ?);
	// 1, 4, 10, 23, 57, 142, 361, 925, 2384, 6160, 15924, 41176, 106493, 275442, 712445, 76800497:76105230 (-695267)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6160, 15924, 41176, 106493, 275442, 712445, 77033225:76352140 (-681085)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2382, 6160, 15924, 41176, 106493, 275442, 712445, 78199852:77619545 (-580307)

	// 1, 4, 10, 23, 57, 142, 361, 925, 2384, 6160, 15924, 41176, 106493, 275442, 712445, 385268917:381825339 (-3443578) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 3000, 6000, ?);
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6160, 15924, 41176, 106493, 275442, 712445, 387933003:384362566 (-3570437)

	// Then

	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6152, 15924, 41176, 106493, 275442, 712445, 219063486:217740789 (-1322697) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 6200, 15800, 1000);
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6151, 15924, 41176, 106493, 275442, 712445, 214480795:213175614 (-1305181)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15924, 41176, 106493, 275442, 712445, 214368433:212978007 (-1390426)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6149, 15924, 41176, 106493, 275442, 712445, 213748534:212455197 (-1293337)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6148, 15924, 41176, 106493, 275442, 712445, 217697938:216533729 (-1164209)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6147, 15924, 41176, 106493, 275442, 712445, 216459512:215261627 (-1197885)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6153, 15924, 41176, 106493, 275442, 712445, 218378637:216973921 (-1404716)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6154, 15924, 41176, 106493, 275442, 712445, 215766432:214450500 (-1315932)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6155, 15924, 41176, 106493, 275442, 712445, 217315241:216057428 (-1257813)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6156, 15924, 41176, 106493, 275442, 712445, 214175927:212818479 (-1357448)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6157, 15924, 41176, 106493, 275442, 712445, 213267907:211931791 (-1336116)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6158, 15924, 41176, 106493, 275442, 712445, 211514199:210251128 (-1263071)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6159, 15924, 41176, 106493, 275442, 712445, 218051476:216914899 (-1136577)

	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15924, 41176, 106493, 275442, 712445, 859700377:854195632 (-5504745) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 6200, 15800, 4000);
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6151, 15924, 41176, 106493, 275442, 712445, 858339497:853075874 (-5263623)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6152, 15924, 41176, 106493, 275442, 712445, 865096410:859839847 (-5256563)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6153, 15924, 41176, 106493, 275442, 712445, 858711753:853240462 (-5471291)
	// 1, 4, 10, 23, 57, 142, 361, 925, 2383, 6154, 15924, 41176, 106493, 275442, 712445, 857376311:851879428 (-5496883) // I thought this might happen DX -- two roads diverge

	// Now I'm going to go back to the drawing board by re-extrapolating based on the two new values
	// Decided on approximately g(k) = g(k-1) * 2.5846 - 1.0868*k, g(6) = 361
	// Which meant I switched to 6150 since that's more correct (and more empirical too) -- I was short-sighted in choosing the other.


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