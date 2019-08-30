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

	// I'll probably refer to this as Pratt^2 (read: Pratt-squared).
	std::vector<long> not_to_be_written_off = // http://oeis.org/A025620 (numbers where 4^i*9^j and i, j >= 0)
	{
		1, 4, 9, 16, 36, 64, 81, 144, 256, 324, 576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976, 147456, 186624, 236196, 262144,
		331776, 419904, 531441, 589824, 746496, 944784
	};
	// worst-case gap distance is (3/2)^2 or 9/4, this is convenient because the ratio of 2s to 3s asymptotically approaches 9/4.
	// The number of 3-smooth numbers up to n is O(lg(n)^2), so it makes sense that squaring them turns what would normally be an O(n*lg(n)^2) algorithm into an something closer to an O(n*lg(n)) algorithm -- I would expect O( n * lg(n) * lg(lg(n)) ) which has to do with the cost of Insertion Sort.
	// The hard part is proving any of this conjecture.
	// Additionally, if you can prove that the sort is perfect asymptotically, you can hybridize it with e.g. Ciura's gap sequence.
	// 1 Million elements: ratio of 2.33078495 ideal -- aside: lg(lg(1000000)) == 4.31698335
	// 100 Thousand elements: ratio of 2.06523435 ideal -- aside: lg(lg(100000)) == 4.05394894
	// 10 Thousand elements: ratio of 1.78367046 ideal -- aside: lg(lg(10000)) == 3.73202085
	// 1 Thousand elements: ratio of 1.47705385 ideal -- aside: lg(lg(1000)) == 3.31698335
	// 1 Hundred elements: ratio of 1.20562513 ideal -- aside: lg(lg(100)) == 2.73202085
	// As an aside, I am really curious about Tokuda's algorithm and how it relates to all of this (it's a really impressive algorithm).

	std::vector<long> hybridized_not_to_be_written_off = // http://oeis.org/A025620 (numbers where 4^i*9^j and i, j >= 0)
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976, 147456, 186624, 236196, 262144,
		331776, 419904, 531441, 589824, 746496, 944784
	}; // This barely changes performance (and I would need to verify it's correct).

	// oh hey, I was reading Pratt's original paper and he gave me a really useful insight, so I am going to test 9^i*25^j (because 5/3 is the largest prime ratio gap)
	std::vector<long> pratt_3_by_5 = // http://oeis.org/A003593 (numbers where 3^i*5^j and i, j >= 0)
	{
		1, 3, 5, 9, 15, 25, 27, 45, 75, 81,
		125, 135, 225, 243, 375, 405, 625, 675, 729, 1125,
		1215, 1875, 2025, 2187, 3125, 3375, 3645, 5625, 6075, 6561,
		9375, 10125, 10935, 15625, 16875, 18225, 19683, 28125, 30375, 32805,
		46875, 50625, 54675, 59049
	};

	// That's exciting :D
	std::vector<long> pratt_squared = // no oeis.org sequence =( (numbers where 9^i*25^j and i, j >= 0)
	{
		1, 9, 25, 81, 225, 625, 729, 2025, 5625, 6561,
		15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441, 1265625
		// TODO: augment with the rest
	};

	std::vector<long> hybridized_pratt_squared = // no oeis.org sequence =( (numbers where 9^i*25^j and i, j >= 0)
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 2025, 5625, 6561,
		15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441, 1265625
		// TODO: augment with the rest
	};
	// It sort of goes without saying: I really like this

	std::vector<long> just_in_case = // http://oeis.org/A003592
	{
		1, 2, 4, 5, 8, 10, 16, 20, 25, 32,
		40, 50, 64, 80, 100, 125, 128, 160, 200, 250,
		256, 320, 400, 500, 512, 625, 640, 800, 1000, 1024,
		1250, 1280, 1600, 2000, 2048, 2500, 2560, 3125, 3200, 4000,
		4096, 5000, 5120, 6250, 6400, 8000, 8192, 10000
	};

	std::vector<long> just_in_case_squared = // 4^i*25^j
	{
		1, 4, 16, 25, 64, 100, 256, 400, 625, 1024,
		1600, 2500, 4096, 6400, 10000, 15625, 16384, 25600, 40000, 62500, // You can tell if a formula is subjectively bad if e.g. you don't hit a million around 20 values
		// TODO?
	};

	// I should probably test 5/11, 7/11, 5/7, and a few others.
	// Maybe some eccentric tests like 17/42
	// Testing non-adjacent primes (or non-primes or co-primes) is sort of important.
	// Testing numbers 50+ will probably stop being useful.
	std::vector<long> test1 = // 5^i*11^j http://oeis.org/A003598
	{
		1, 5, 11, 25, 55, 121, 125, 275, 605, 625,
		1331, 1375, 3025, 3125, 6655, 6875, 14641, 15125, 15625, 33275,
		34375, 73205, 75625, 78125, 161051, 166375, 171875, 366025, 378125, 390625,
		805255, 831875, 859375, 1771561, 1830125, 1890625
	};

	std::vector<long> test2 = // 25^i*121^j
	{
		1, 25, 121, 625, 3025, 14641, 15625, 75625, 366025, 390625, // even if it doesn't seem like it's gonna work, you still have to try
		1771561
	};

	// a lot of this is arbitrary
	std::vector<long> test3 = // 25^i*121^j with interpolation between values (quasi-geometric average of adjacent nodes (trying to minimize ratio on both sides, except on first gap), but only if the gap exceeds 2.2 (11/5))
	{
		// The interpolation formula: ceil(a*sqrt(b/a))
		// An aside, as long as you add a constant number of interpolation nodes between elements, time complexity should not change drastically.
		1, 4, 11,
		25, 55,
		121, 275,
		625, 1375,
		3025, 6655,
		14641, // first value with no need for interpolation
		15625, 34375,
		75625, 166375,
		366025,
		390625, 831875,
		1771561
	};

	// Best formula yet. Within striking distance of Ciura's formula (at least in terms of the asymptotic version because of its hybrid nature).
	std::vector<long> test4 = // Ciura hybridized - 25^i*121^j with interpolation between values (quasi-geometric average of adjacent nodes (trying to minimize ratio on both sides, except on first gap), but only if the gap exceeds 2.2 (11/5))
	{
		// The interpolation formula: ceil(a*sqrt(b/a))
		// An aside, as long as you add a constant number of interpolation nodes between elements, time complexity should not change drastically.
		1, 4, 10, 23, 57, 132, 301, 701, 1750,
		3025, 6655,
		14641, // first value with no need for interpolation
		15625, 34375,
		75625, 166375,
		366025, // if only I could "get rid of" (you can't actually do that, at least not trivially) these gaps I could probably beat Ciura's sequence
		390625, 831875,
		1771561
	};

	// Awww =( (it wasn't going to be trivial)
	// I'll need to see if there's a nice algorithm for combining gaps that are too close (it would need extra contextual information)
	std::vector<long> test5 = // Ciura hybridized - with even more arbitrary going on- 25^i*121^j with interpolation between values (quasi-geometric average of adjacent nodes (trying to minimize ratio on both sides, except on first gap), but only if the gap exceeds 2.2 (11/5))
	{
		// The interpolation formula: ceil(a*sqrt(b/a))
		// An aside, as long as you add a constant number of interpolation nodes between elements, time complexity should not change drastically.
		1, 4, 10, 23, 57, 132, 301, 701, 1750,
		3025, 6655,
		//14641, // first value with no need for interpolation
		15125, // how's that for trivial
		/*15625,*/ 34375,
		75625, 166375,
		//366025, // if only I could "get rid of" (you can't actually do that, at least not trivially) these gaps I could probably beat Ciura's sequence
		378125, // how's that for trivial
		/*390625,*/ 831875,
		1771561
	};
}