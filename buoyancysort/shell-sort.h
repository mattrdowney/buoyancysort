#pragma once

#include <algorithm>
#include <stddef.h>
#include <vector>

namespace ShellSort
{
	// TODO: I'm beginning to suspect I have a subtle bug that's being caught by the insertion sort at the end...
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1) // FIXME: I am an idiot // Well, I might as well conform to how sequences usually work and just reprogram the data.
		{
			long gap = gap_sequence[gap_index];
			for (long unsorted_cursor = (before_first+1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
			{
				Type temp = data[unsorted_cursor];
				long insertion_cursor;
				for (insertion_cursor = unsorted_cursor; insertion_cursor - (before_first + 1) >= gap && data[insertion_cursor - gap] > temp; insertion_cursor -= gap)
				{
					data[insertion_cursor] = data[insertion_cursor - gap];
				}
				data[insertion_cursor] = temp;
			}
		}
	}

	// If you don't want to use the operating system's stack, then this would hypothetically be a good option.
	template <typename Type>
	void lazy_sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		// Start with InsertionSort::lazy_leftward_sort(data, before_first, after_last, [](long n){return 2*n;});
		// If done, return
		// Otherwise lazily shell sort, the first iteration might be straightforward-ish, but the rest have substantial room for optimization (in this case, I'm actually interested in optimizing)
		// Optimization: keep track of the leftmost "first unsorted" node
		// Optimization: increase function (gap sequence number) when a threshold like 2n has been crossed.
		//    NOTE: why is this important? You don't want to sort in e.g. O(nlglgn) when the same work could be done in O(k)
		// Optimization: any time an insertion happens you can probably ignore the gap number and just count the number of insertions
		// Optimization(?): you could consider having thresholds go forwards and backwards (gap sequence number can reduce as well depending on budget)
		//    E.g. if items are sorted within a range of 64 places of their correct position and the gap sequence number of 65 continuously fails, then the number should reduce accordingly.
		//    Similarly if the gap sequence number is 5 and it often shifts 10 places (50 among the 64) then the order should be increased accordingly.
		// When you hit the end, you start from the "first unsorted" node and scan forwards with a gap sequence number of 1 again.
		//    If you ever hit the end with a gap sequence of 1, then the series is sorted.
		// A node is eligible to be a "first unsorted" node when it is inserted with a gap number other than 1 (because it could be imprecise).
		// When you increase the gap sequence number, you have a guarantee you have travelled a substantial distance, so subtracting the difference between the next gap number and the current gap number should not go out of bounds (although you should still do boundary checking because it should fail occasionally).
		// Optimization: when budget exceeds above or dwindles below it's Goldilocks zone, you should switch from one gap sequence number to another.
	}

	// This presumably uses local optimization (taking small subproblems and building off of them), so if you want to beat it you probably want to rely on global optimization
	std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701, 1750 };

	std::vector<long> pratt_three_smooth_gap_sequence =
	{
		1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27, 32, 36, 48, 54, 64, 72, 81, 96, 108, 128, 144, 162, 192, 216, 243,
		256, 288, 324, 384, 432, 486, 512, 576, 648, 729, 768, 864, 972, 1024, 1152, 1296, 1458, 1536, 1728, 1944,
		2048, 2187, 2304, 2592, 2916, 3072, 3456, 3888
	};

	std::vector<long> tokuda_gap_sequence =
	{
		1, 4, 9, 20, 46, 103, 233, 525, 1182, 2660, 5985, 13467, 30301, 68178, 153401, 345152, 776591, 1747331,
		3931496, 8845866, 19903198, 44782196, 100759940, 226709866, 510097200, 1147718700
	};

	std::vector<long> sedgewick_incerpi_sequence =
	{
		1, 3, 7, 21, 48, 112, 336, 861, 1968, 4592, 13776, 33936, 86961, 198768, 463792, 1391376, 3402672, 8382192,
		21479367, 49095696, 114556624, 343669872, 852913488, 2085837936
	};

	std::vector<long> n_factorial =
	{
		1, 2, 6, 24, 120, 720, 5040, 40320
	};

	std::vector<long> n_to_the_power_of_n =
	{
		1, 4, 27, 256, 3125, 46656
	};

	std::vector<long> n_over_2_to_the_power_of_n_over_2 =
	{
		1, 4, 10, 27, 81, 256, 870, 3125, 11804, 46656
	};

	// This works okay
	// std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701, 1750 };
	std::vector<long> n_over_3_halves_plus_euler_mascheroni_to_the_power_of_n_over_3_halves_plus_euler_mascheroni =
	{
		1, 4, 9, 22, 60, 181, 574, 1930
	};

	std::vector<long> n_over_sqrt_5_to_the_power_of_n_over_sqrt_5 =
	{
		1, 7, 15, 36, 96, 272, 812, 2534
	};

	//
	std::vector<long> test =
	{
		1, 5, 13, 29, 68, 173
	};

	// old idea:
	std::vector<long> test2 = // floor(sqrt(5)^n) - 1
	{
		1, 4, 10, 24, 54, 124, 278, 624
	};

	// std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701, 1750 };
	std::vector<long> test3 = // basically (n/2.09)^(n/2.09) -- does worse
	{
		1, 4, 9, 21, 58, 171, 538, 1791
	};

	// Best so far: Well this can beat ciura_gap_sequence for n=1000 seed=0
	// Algorithm: ceil((n/2.05)^(n/2.05))
	// And to think: I never would have found this if I weren't a dumbass. (Okay, that isn't necessarily true but being a dumbass definitely helped.)
	std::vector<long> test4 =
	{
		1, 4, 9, 24, 67, 204, 662, 2276
	};
}