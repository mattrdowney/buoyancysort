#pragma once

#include <algorithm>
#include <stddef.h>
#include <vector>

namespace ShellSort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
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

	std::vector<long> ciura_gap_sequence = { 1750, 701, 301, 132, 57, 23, 10, 4, 1 }; // NOTE: 1750 is an (un-official?) extension

	std::vector<long> root_five_silver_integer_with_memory_gap_sequence = { 117653, 52445, 23413, 10408, 4628, 2029, 904, 404, 173, 68, 29, 13, 5, 1 };
	// This is my best version so far. Algorithm: N(i) = (ceil(sqrt(floor(N(i-1)*sqrt(5))-4)))^2+4
	// In abstract it multiplies by sqrt(5) and gets the next number of the form n^2 + 4 (a "silver integer" from a silver ratio)
	// Performance on N=100'000 objects -- 714'668'030 comparisons (which is pretty good -- ~27588015.7 comparisons would be expected for N*lg(N)^2 (this is ~26x that number))
	// I feel like this gap sequence has a performance of N*lg(N)^3 (although I could easily be wrong).

	// I know this gap sequence is supposedly N*lg(N)^2, but I'd still like to test it myself. Unfortunately, I don't feel like figuring out more 3-smooth numbers (can't find any more online).
	std::vector<long> pratt_three_smooth_gap_sequence = // hopefully there are no typos
	{
		3888, 3456, 3072, 2916, 2592, 2304, 2187, 2048, 1944, 1728, 1536, 1458, 1296, 1152, 1024, 972, 864,
		768, 729, 648, 576, 512, 486, 432, 384, 324, 288, 256, 243, 216, 192, 162, 144, 128, 108, 96, 81, 72, 64, 54, 48, 36, 32, 27,
		24, 18, 16, 12, 9, 8, 6, 4, 3, 2, 1
	};

	std::vector<long> fibonacci_silver_integer_gap_sequence = { 2029, 1229, 733, 445, 260, 148, 85, 53, 29, 13, 5, 1 };

	std::vector<long> root_five_primes_with_memory_gap_sequence = { 8923, 3989, 1777, 787, 347, 151, 67, 29, 11, 5, 1 };

	std::vector<long> root_five_silver_integer_with_memory_prioritize_primes_gap_sequence = { 733, 365, 173, 68, 29, 13, 5, 1 };

	std::vector<long> intuition_based_testing_gap_sequence1 = { 732, 327, 146, 65, 29, 13, 5, 1 }; // only focusing on sqrt(5) factor after 29
	std::vector<long> intuition_based_testing_gap_sequence2 = { 965, 409, 173, 71, 29, 13, 5, 1 }; // alternating between primes and "silver integers" after 29
	// This algorithm (which on ~half of all iterations finds the next prime) seems to do a little better.

	// The thing that interests me the most here is that the performance seems to start at O(N*lg(N)^2) and devolve to O(N*lg(N)^3).

	std::vector<long> intuition_based_testing_gap_sequence3 = { 509, 253, 125, 61, 29, 13, 5, 3, 1 }; // powers of 2 minus 3

	std::vector<long> tokuda_gap_sequence =
	{
		1147718700, 510097200, 226709866, 100759940, 44782196, 19903198, 8845866, 3931496, 1747331, 776591,
		345152, 153401, 68178, 30301, 13467, 5985, 2660, 1182, 525, 233, 103, 46, 20, 9, 4, 1
	};

	std::vector<long> two_and_a_quarter_silver_integer_with_memory_gap_sequence = { 11453, 5045, 2213, 965, 404, 173, 68, 29, 11, 5, 1 };
}