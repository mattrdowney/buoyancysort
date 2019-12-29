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
	std::vector<long> gap_sequence2 = ShellSort::new_shellsort_gap_sequence;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	//ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 45000, 120000, 40991, 41200, 41);
	ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 500);
	//ShellSort::empirical_comparison_inner_range(gap_sequence1, gap_sequence1, 42000, 100000, 41326, 41326+1, 41326+50, 41);

	//1, 3, 8, 19, 47, 117, 289, 1455, 7331, 36987, 186619, 941531, 881834461:992186266 (110351805) //ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 500); -- compare to my best sequence
	//1, 3, 8, 19, 47, 117, 289, 1455, 7331, 36987, 186619, 941531, 937280686:1151640007 (214359321) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 200000, 1000000, 50); -- compare to Tokuda

	// then add intermediate values -- no multiplier of ~5 after 289

	// 1, 3, 8, 19, 47, 117, 289, 1455, 3266, 7331, 16467, 36987, 83081, 186619, 419175, 941531, 895795456:925633717 (29838261) // ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 200000, 1000000, 50); -- compare to Tokuda
	// 1, 3, 8, 19, 47, 117, 289, 1455, 3266, 7331, 16467, 36987, 83081, 186619, 419175, 941531, 873949013:906853855 (32904842) //ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 500); -- compare to my best sequence

	// doesn't seem to work that well. Switching back to old concept

	// 1, 4, 10, 23, 57, 145, 364, 917, 2312, 5832, 14711, 37113, 93625, 236190, 595844, 884728670:888618278 (3889608) //ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 500); -- compare to my best sequence

	// Even that doesn't work well. Well, at least number theory + ShellSort is an interesting and challenging problem.
	// If I knew the asymptotic multiplier I think the problem would be a lot easier, but where's the fun in that.

	// Just in case the error (missing value between 289 and 1455) changed the outcome:
	// 1, 3, 8, 19, 47, 117, 289, 648, 1455, 3266, 7331, 16467, 36987, 83081, 186619, 419175, 941531, 873060738:885654654 (12593916) //ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 42000, 100000, 500); -- compare to my best sequence

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