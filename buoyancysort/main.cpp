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
	std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda2;
	//std::vector<long> gap_sequence2 = ShellSort::gap_sequence_generator(ShellSort::extrapolated_ciura, 20);
	//std::vector<long> gap_sequence2 = ShellSort::extrapolated_ciura_tokuda3;
	ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 900, 1700, 820, 840, 1000);
	//ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 100000, 1000000);

	//1, 5, 19, 41, 107, 125020337 : 128628348 (3608011) // ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 100, 200, 107, 125, 100000);
	//1, 5, 19, 41, 108, 125021537 : 128314088 (3292551)
	//1, 5, 19, 41, 109, 125020572 : 128281310 (3260738)
	//1, 5, 19, 41, 110, 125017201 : 128345207 (3328006)
	//1, 5, 19, 41, 111, 125013300 : 128251590 (3238290)
	//1, 5, 19, 41, 112, 125019787 : 128224768 (3204981)
	//1, 5, 19, 41, 113, 125025006 : 128309917 (3284911)
	//1, 5, 19, 41, 114, 125015867 : 128246286 (3230419)
	//1, 5, 19, 41, 115, 125018572 : 128351712 (3333140)
	//1, 5, 19, 41, 116, 125033066 : 128340513 (3307447)
	//1, 5, 19, 41, 117, 125020337 : 128574748 (3554411)
	//1, 5, 19, 41, 118, 125021082 : 128469637 (3448555)
	//1, 5, 19, 41, 119, 125020042 : 128347043 (3327001)
	//1, 5, 19, 41, 120, 125033066 : 129008265 (3975199)
	//1, 5, 19, 41, 121, 125018334 : 128400961 (3382627)
	//1, 5, 19, 41, 122, 125032160 : 128404168 (3372008)
	//1, 5, 19, 41, 123, 125020633 : 129295561 (4274928)
	//1, 5, 19, 41, 124, 125019942 : 128317621 (3297679)
	//1, 5, 19, 41, 125, 125020342 : 128286152 (3265810)

	//1, 5, 17, 41, 109, 157828947:165863971 (8035024) // ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 100, 200, 109, 113, 100000);
	//1, 5, 17, 41, 110, 157829190 : 164030490 (6201300)
	//1, 5, 17, 41, 111, 157835999 : 165983636 (8147637)
	//1, 5, 17, 41, 112, 157828245 : 164284359 (6456114)
	//1, 5, 17, 41, 113, 157829652 : 165874446 (8044794)

	//1, 5, 19, 41, 112, 305, 820, 17891857:18359803 (467946) // ShellSort::empirical_comparison_range(gap_sequence1, gap_sequence2, 900, 1700, 820, 840, 1000); // As I thought, this gap sequence is basically 41*e^(k) after 41 (although it might diverge at some point).
	//1, 5, 19, 41, 112, 305, 821, 17920035 : 18399508 (479473)
	//1, 5, 19, 41, 112, 305, 822, 17903060 : 18392891 (489831)
	//1, 5, 19, 41, 112, 305, 823, 17832986 : 18341122 (508136)
	//1, 5, 19, 41, 112, 305, 824, 17866578 : 18374682 (508104)
	//1, 5, 19, 41, 112, 305, 825, 17932329 : 18415386 (483057)
	//1, 5, 19, 41, 112, 305, 826, 17842839 : 18344081 (501242)
	//1, 5, 19, 41, 112, 305, 827, 17907053 : 18389342 (482289)
	//1, 5, 19, 41, 112, 305, 828, 17805850 : 18261024 (455174)
	//1, 5, 19, 41, 112, 305, 829, 17854082 : 18332762 (478680)
	//1, 5, 19, 41, 112, 305, 830, 17804541 : 18279711 (475170)
	//1, 5, 19, 41, 112, 305, 831, 17784909 : 18272014 (487105)
	//1, 5, 19, 41, 112, 305, 832, 17921425 : 18401610 (480185)
	//1, 5, 19, 41, 112, 305, 833, 17785154 : 18290814 (505660)
	//1, 5, 19, 41, 112, 305, 834, 17814861 : 18416986 (602125)
	//1, 5, 19, 41, 112, 305, 835, 17763890 : 18247768 (483878)
	//1, 5, 19, 41, 112, 305, 836, 17806077 : 18281604 (475527)
	//1, 5, 19, 41, 112, 305, 837, 17808413 : 18294020 (485607)
	//1, 5, 19, 41, 112, 305, 838, 17926572 : 18397522 (470950)
	//1, 5, 19, 41, 112, 305, 839, 17879666 : 18353603 (473937)
	//1, 5, 19, 41, 112, 305, 840, 17919152 : 18383965 (464813)

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