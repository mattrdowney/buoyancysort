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

	std::vector<long> primes_squared = // (and 1)
	{
		1, 4, 9, 25, 49, 121, 169, 289, 361, 529, 841, 961, 1369, 1681, 1849, 2209, 2809, 3481, 3721, 4489, 5041, 5329,
		6241, 6889, 7921, 9409, 10201, 10609, 11449, 11881, 12769, 16129, 17161, 18769, 19321, 22201, 22801, 24649,
		26569, 27889, 29929, 32041, 32761, 36481
	};

	std::vector<long> test1 = //T(n) = next_prime[ceil(T(n-1)*sqrt(31/6))]
	{
		//1, 4, 10, 23, 53+4, 130+2, 301, 685+16, 1594+106
		1, 4, 10, 23, 53, 121, 276, 628
	};

	//{ 1, 4, 10, 23, 57, 132, 301, 701, 1750 };
	// 1 
	// 2*2
	// 2*5
	// 23
	// 3*19 NOT 53 || 2*3*3*3 || 5*11  || 2*2*2*7
	// 2*2*3*11 NOT 2*5*13 || 131
	// 7*43
	// 701 NOT 5*137 || ...
	// 2*5*5*5*7 NOT 

	// Value of T(n)^(1/(n-1))
	// 4 -- n=1
	// 3.16227766 -- n=2
	// 2.84386698 -- n=3
	// 2.74769621 -- n=4
	// 2.65530728
	// 2.58877566
	// 2.54994695
	// 2.54319556

	// Differencing of prior values
	// 0.83772234
	// 0.31841068
	// 0.09617077
	// 0.09238893
	// 0.06653162
	// 0.03882871
	// 0.00675139

	//Differencing of prior values (x2)
	// 0.51931166
	// 0.22223991
	// 0.00378184
	// 0.02585731
	// 0.02770291
	// 0.03207732

	//Differencing of prior values (x3)
	// 0.29707175
	// 0.21845807
	// -0.02207547
	// -0.0018456
	// -0.00437441

	//Differencing of prior values (x4)
	// 0.07861368
	// 0.24053354
	// -0.02022987
	// 0.00252881

	//Differencing of prior values (x5)
	// -0.16191986
	// 0.26076341
	// -0.02275868

	//Differencing of prior values (x6)
	//  -0.42268327
	// 0.28352209

	//Differencing of prior values (x7)
	// -0.70620536

	// Approximate via a polynomial of degree 1:
	// Discrete derivative:
	// 2.54994695 = a*7 + b,
	// 2.54319556 = a*8 + b
	// b = 2.597207, a = -0.006751404

	// Thus my series continuation looks like:
	// (a*n + b)^n, b = 2.597207, a = -0.006751404, n=8

	// 1, 4, 10, 23, 57, 132, 301, 701, 1750 [, 4346, 10732, 26362, 64406, 156493, 378175, 908869]

	// worse than Tokuda asymptotically but similar performance
	std::vector<long> extrapolated_ciura_gap_sequence =
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 4346, 10732, 26362, 64406, 156493, 378175, 908869
	};

	// Tokuda reference: 1, 4, 9, 20, 46, 103, 233, 525, 1182, 2660, 5985, 13467, 30301, 68178, 153401, 345152, 776591, 1747331,
	//3931496, 8845866, 19903198, 44782196, 100759940, 226709866, 510097200, 1147718700

	// (4^(1/lg(n)))*(2.5^(1-1/lg(n)))
	// geometric mean between 4 and 2.5 with n=1 
	// First attempt at formula: ((4^(1/(ln(n)/ln(k))))*(2.25^(1-1/(ln(n)/ln(k)))))^(n-k), n=15+k, k=1.55
	// Thus this extrapolation yields:
	std::vector<long> extrapolated_ciura_gap_sequence2 =
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 3873, 9320, 22398, 53751, 128842, 308509, 738012
	};
	// This worked pretty well; hopefully it's a step in the right direction -- until I beat the extrapolated Ciura sequence this isn't particularly interesting

	std::vector<long> extrapolated_ciura_gap_sequence3 = // ((4^(1/(ln(n)/ln(k))))*(2.25^(1-1/(ln(n)/ln(k)))))^(n-k), n=15+k, k=1.5 // worse
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1504, 3609, 8642, 20666, 49354, 117736, 280590, 668120
	};

	std::vector<long> three_smooth_subsequence1 = // * 2, * 2, * 3 (and repeat) version
	{
		1, 2, 4, 12, 24, 48, 144, 288, 576, 1728, 3456, 6912, 20736, 41472, 82944, 248832, 497664, 995328, 2985984
	};

	std::vector<long> three_smooth_subsequence2 = // procedural version
	{
		1, 2, 4, 12, 24, 48, 144, 288, 576, 1152, 3456, 6912, 13824, 27648, 82944, 165888, 331776, 995328, 1990656
	};
}