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

	// This presumably uses local optimization (taking small subproblems and building off of them), so if you want to beat it you probably want to rely on global optimization
	std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701,
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
		1577 /*1750*/, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711 };

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

	// (4^(1/lg(n)))*(2.5^(1-1/lg(n)))
	// geometric mean between 4 and 2.5 with n=1 
	// First attempt at formula: ((4^(1/(ln(n)/ln(k))))*(2.25^(1-1/(ln(n)/ln(k)))))^(n-k), n=15+k, k=1.55
	// Thus this extrapolation yields:
	std::vector<long> extrapolated_ciura_gap_sequence2 =
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 3873, 9320, 22398, 53751, 128842, 308509, 738012
	};
	// This worked pretty well; hopefully it's a step in the right direction -- until I beat the extrapolated Ciura sequence this isn't particularly interesting


	std::vector<long> four_nine_smooth = // http://oeis.org/A025620 (numbers where 4^i*9^j and i, j >= 0)
	{
		1, 4, 9, 16, 36, 64, 81, 144, 256, 324, 576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976, 147456, 186624, 236196, 262144,
		331776, 419904, 531441, 589824, 746496, 944784
	};

	std::vector<long> test1 = // http://oeis.org/A025620 (numbers where 4^i*9^j and i, j >= 0)
	{
		1, 4, 9, 16, 36, 64, 144, 256, 576, 1296, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976, 147456, 186624, 236196, 262144,
		331776, 419904, 531441, 589824, 746496, 944784
	};

	
	// The problem with this strategy is it means all values are 2^{0,2}*3^k
	// Try to multiply current number by 9/4
	// If you cannot, take the previous number and multiply it by 4
	std::vector<long> test2 =
	{
		1, 4, 9, 16, 36, 81, 144, 256, 576, 1296, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976, 147456, 186624, 236196, 262144,
		331776, 419904, 531441, 589824, 746496, 944784
	};

	std::vector<long> test3 = // 1, 4, T(n)=ceil(T(n-1)*(3^2/2^2)+epsilon) -- worse than Tokuda and (of course) Ciura
	{
		1, 4, 9, 23, 52, 118, 266, 599, 1348, 3034, 6827, 15361, 34563, 77767, 174976, 393697, 885819
	};

	std::vector<long> test4 = // 1, 4, T(n)=ceil(T(n-1)*(3^2/2^2)+epsilon) -- worse than Tokuda and (of course) Ciura
	{
		1, 4, 9, 21, 48, 118, 266, 599, 1348, 3034, 6827, 15361, 34563, 77767, 174976, 393697, 885819
	};

	// n = 1, 4, 9, ceil((9*2.25^(k-3))), k=4+

	std::vector<long> test5 = // 1, 4, T(n)=ceil(T(n-1)*(3^2/2^2)+epsilon) -- worse than Tokuda and (of course) Ciura
	{
		1, 4, 9, 21, 46, 103, 231, 519, 1168, 2628, 5912, 13302, 29928
	};

	//v = ((4 ^ (1 / (ln(n) / ln(k))))*(2.25 ^ (1 - 1 / (ln(n) / ln(k))))) ^ (n - k + 1), n = 4 + k, k = 1.56
	std::vector<long> test6 =
	{
		1, 4, 9, 21, 55, 133, 324, 787, 1912, 2628, 5912, 13302, 29928
	};

	// Basically Tokuda with a small change (instead of /5 it's /2.25/2.25)
	std::vector<long> test7 = // ceil((10*2.25^(k-1) - 4)/2.25/2.25)
	{
		1, 4, 9, 20, 45, 102, 230, 519, 1167, 2627, 5911, 13301, 29927
	};

	// Basically Tokuda with a small change again
	// The basic idea is to do a perfect split of the 9 down the middle, hence 9/2
	std::vector<long> test8 = // ceil((9*2.25^(k-1) - 4)/(9/2)), k=5 [first index doesn't quite work but that's fine]
	{
		1, 4, 10, 22, 51, 115, 259, 583, 1313, 2955, 6650, 14963, 33668
	};

	// Basically Tokuda with a small change again.
	// The basic idea is to pick a irrational ratio (no particular one -- so long as it is dissimilar from 2,3) somewhere in the (4,5) range.
	std::vector<long> test9 = // ceil((9*2.25^(k-1) - 4)/sqrt(21)), k=5 [first index doesn't quite work but that's fine]
	{
		1, 4, 11, 24, 55, 125, 283, 637, 1433, 3226, 7258, 16331, 36745
	};

	// Basically Tokuda with a small change again.
	std::vector<long> test10 = // ceil((9*2.25^(k-1) - 4)/sqrt(21)), k=5 [first index doesn't quite work but that's fine]
	{
		1, 4, 11, 26, 60, 137, 310, 699, 1575, 3545, 7979, 17955, 40400
	};

	// Basically Tokuda with a small change again.
	std::vector<long> test11 = // ceil((9*2.25^(k-1) - 4)/4.95), k=5 [first index doesn't quite work but that's fine]
	{
		1, 4, 11, 20, 46, 105, 236, 530, 1194, 2687, 6046, 13603, 30607
	};

	//T(n) = (T(n-1) - floor(T(n-1)/2.25))*(T(n-1)/(T(n-1)-ceil(T(n-1)/2.25)))*2.25, n=4

	// Tokuda had to try this, right?
	std::vector<long> test12 = // ceil((9*2.25^(k-1) - x)/(9-x)), k=1, x= 5
	{
		1, 4, 11, 25, 57, 129, 291, 656, 1477, 3325, 7481, 16833, 37876
	};

	// Similar-ish
	std::vector<long> test13 = // ceil((9*2.25^(k-1) - x)/(9-x)), k=1, x= 4.5
	{
		1, 4, 10, 22, 51, 115, 259, 583, 1313, 2955, 6650, 14963, 33668
	};

	// ceil((n - ceil(n/9))*(n/(n-ceil(n/9)))*2.25), n=9
	std::vector<long> test14 = // ceil((9*2.25^(k-1) - x)/(9-x)), k=1, x= 4.5
	{
		1, 4, 9, 21, 48, 108, 243, 547, 1231, 2770, 6233, 14025, 31557
	};
}