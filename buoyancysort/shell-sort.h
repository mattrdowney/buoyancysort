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
			for (long unsorted_cursor = (before_first + 1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
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

	std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701, // FIXME: I should add 1750 (after testing that's better) for a fairer comparison
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
		//1750 /*not legit*/, 3937, 8858, 19930, 44842, 100894, 227011, 510774, 1149241 };
		1577, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711 };

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

	// This formula is more
	std::vector<long> hybridized_pratt_3_5_squared = //  http://oeis.org/A003593 (but squared) sequence (numbers where 9^i*25^j and i, j >= 0)
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 2025, 5625, 6561,
		15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441, 1265625
		// TODO: augment with the rest
	};

	// Best formula yet. Within striking distance of Ciura's formula (at least in terms of the asymptotic version because of its hybrid nature).
	std::vector<long> hybridized_pseudo_pratt_5_11_squared = // Ciura hybridized - 25^i*121^j with interpolation between values (quasi-geometric average of adjacent nodes (trying to minimize ratio on both sides, except on first gap), but only if the gap exceeds 2.2 (11/5))
	{
		// The interpolation formula: ceil(a*sqrt(b/a))
		// An aside, as long as you add a constant number of interpolation nodes between elements, time complexity should not change drastically.
		1, 4, 10, 23, 57, 132, 301, 701, 1750,
		//1375, 2040, // I mostly wasn't sabotaged here. 1750 generally worsens Ciura's formula (it seems to worsen asymptotic behavior), but it helps me somehow
		3025, 6655,
		14641, // first value with no need for interpolation
		15625, 34375,
		75625, 166375,
		366025, // if only I could "get rid of" (you can't actually do that, at least not trivially) these gaps I could probably beat Ciura's sequence
		390625, 831875,
		1771561
	};
}