#pragma once

#include <algorithm>
#include <iterator>
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

	std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701,
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
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

	std::vector<long> hybridized_pratt_3_5_squared = //  http://oeis.org/A003593 (but squared) sequence (numbers where 9^i*25^j and i, j >= 0)
	{
		1, 4, 10, 23, 57, 132, 301, 701, 1750, 2025, 5625, 6561,
		15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441, 1265625
	};

	// Best formula yet. Within striking distance of Ciura's formula (at least in terms of the asymptotic version because of its hybrid nature).
	std::vector<long> hybridized_pseudo_pratt_5_11_squared = // Ciura hybridized - 25^i*121^j with interpolation between values (quasi-geometric average of adjacent nodes (trying to minimize ratio on both sides, except on first gap), but only if the gap exceeds 2.2 (11/5))
	{
		// The interpolation formula: ceil(a*sqrt(b/a))
		1, 4, 10, 23, 57, 132, 301, 701, 1750,
		3025, 6655,
		14641, // first value with no need for interpolation
		15625, 34375,
		75625, 166375,
		366025, // if only I could get rid of these gaps I could probably beat Ciura's sequence
		390625, 831875,
		1771561
	};

	// Future note related to practicality:
	// Most of the problems I encounter with the sequences are due to gaps getting too small.
	// There are two ways to fix this:
	// Strategy 1: make a/b ~= 1 (it won't fix anything asymptotically (I think?) but it's useful in practical terms). // I think this is patently false, this just ensures the worst case scenerio always happens. // Wait maybe this is what you want (for a different reason). Distant ratios basically waste a lot of potential work (at least at the level of e.g. 2/101).
	// Strategy 2: make a/b ~= irrational (the randomness means more useful work). Note: I think phi would backfire since phi+1=phi^2 // I think I was wrong about the phi part. E.g. I used 3/5 which approximates phi really well.
	// Using Fibonacci primes might not be a bad idea (especially if you could find some that had a ~1:k:k^2:k^3 ratio between a^2*b^2 : a^4*b^2 : a^2*b^4 : a^4*b^4
	// Based on my tests this would mean b/a = k, a*b = k^2, k=e.g.2 (which when solved says a:b is 1:2, which was my intuition). Now I just need the right primes.
	// Oh wait, but that's only if you weren't going to square the result. You want a ratio of 1:sqrt(2):2:2*sqrt(2) -- which after squaring becomes 1:2:4:8
	// The best approximation for that is probably 7/5 (~=sqrt(2)).
	// Thus you would have something like 1:25:49:1225 (actually that's not right)
	// Let's try this differently 1 : 4(smallest prime squared) : 9/25 (closest co-prime to 16 in this case)
	// Better sequences must exist 1 : 9 : 64/100 (closest co-prime to 81 in this case) : 576/900
	// You could try 16, 25, 36 onwards but the problem with squares is they will still be within lg(n) distance if you can't use the number in question.
	// This does make me wonder: can I use irrational numbers ceiled? (my intuition says no)
	// I think I am over-analyzing the problem at this point
	// That being said, I was just looking at the squares and I think there's a good candidate 3^2 and (2*5)^2
	// Sure, there's a significant gap between 9 and 100, but you should be able to interpolate (I think).

	std::vector<long> longshot = // 4^i*64^j -- there's no way this could work / nevermind
	{
		1, 4, 16, 64, 256, 1024, 4096, // Funny enough this is the 2^k shellsort floor(N/2^k) is the textbook definition.
	};

	std::vector<long> actual_good_idea1 = // 3^i*8^j
	{
		1, 3, 8, 9, 24, 27, 64, 72, 81, 192,
		216, 243, 512, 576, 648, 729, 1536, 1728, 1944, 2187,
		4096, 4608, 5184, 5832, 6561, 12288, 13824, 15552, 17496, 19683,
		32768, 36864, 41472, 46656, 52488, 59049, 98304, 110592, 124416, 139968,
		157464, 177147, 262144
	};

	std::vector<long> actual_good_idea2 = // 9^i*64^j // but I think this idea isn't worth trying.
	{
		1, 9, 64, 81, 576, 27, 64, 72, 81, 192,
		216, 243, 512, 576, 648, 729, 1536, 1728, 1944, 2187,
		4096, 4608, 5184, 5832, 6561, 12288, 13824, 15552, 17496, 19683,
		32768, 36864, 41472, 46656, 52488, 59049, 98304, 110592, 124416, 139968,
		157464, 177147, 262144
	};

	std::vector<long> generalized_pratt(std::set<long> factors_list, long values)
	{
		// Using ordered sets makes the implementation user-proof (sort of).
		// Duplicate and misordered entries aren't a problem, although negative numbers still are.
		// While negative numbers may first appear to be bad, I don't think it would be meaningless to augment the algorithm to account for them.
		// The only real change is the invariant where the leftmost partitioned elements never changed goes away
		// And is replaced by an outwards expansion from zero (where the "middle" elements never change).
		factors_list.insert(1);
		if (factors_list.size() == 1)
		{
			// fancy way of saying "return {1}"
			return std::vector<long>(factors_list.begin(), factors_list.end());
		}
		// The following code guards against user errors when: (user-proofing / defensive programming)
		//     1) values < factors_list.size() or
		//     2) the largest factor is greater than the nth value
		std::set<long> unexponentiated(factors_list.begin(), factors_list.end());
		for (int guaranteed_value = 0; guaranteed_value < values-1; guaranteed_value += 1)
		{
			// Set cannot be modified while iterators are traversing, but we can avoid that altogether.
			long current_value = *std::next(unexponentiated.begin(), guaranteed_value);
			for (long factor : factors_list) // this includes 1 but I don't mind
			{
				unexponentiated.insert(current_value*factor);
			}
		}

		// For e.g. a Pratt<2,3> you want to square the sequence to 4,9
		// For higher powers, I don't think it's quite the same
		// E.g. for a Pratt<2,3,5> I think you don't want numbers in the same fashion. Hence this code.
		int power = factors_list.size() - 1;
		std::vector<long> exponentiated;
		for (int index = 0; index < values; index += 1)
		{
			long value = *std::next(unexponentiated.begin(), index);
			long result = 1;
			// kinda equivalent to pow(value, power);
			for (int lazy_power = 0; lazy_power < power; lazy_power += 1)
			{
				result *= value;
			}
			exponentiated.push_back(result);
		}

		return exponentiated;
	}

	// My intuition says the best path forward is to work on the 4^i*9^j version. (Someone else can always test other possibilities; I would want to write code to test them via brute force if I actuallly went down that path)
}