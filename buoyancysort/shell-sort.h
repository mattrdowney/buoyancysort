#pragma once

#include <algorithm>
#include <climits>
#include <functional>
#include <iterator>
#include <stddef.h>
#include <vector>

namespace ShellSort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence, long max_size = LONG_MAX)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
		{
			long gap = gap_sequence[gap_index];
			if (gap > max_size)
			{
				continue;
			}
			for (long unsorted_cursor = (before_first + 1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
			{
				int a;
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
		// Extra notes: I think doubling the gap distance is generally a good idea; you could slowly interpolate but there's a lot of negative side effects.
		// You should cap the gap sequence on the 2nd and later iterations to be one less than the max on the prior iterations.
		// You should never lower your gap number early (i.e. not until you hit the end of file and need to do another pass).
		// I also think this might be a lot slower, especially if you have to backtrack and validate the sortedness of arrays in the middle of the broadphase passes multiple times.
		// I suppose you could store auxiliary information about the sortedness (with ~O(lgn) bits of information), but one rogue insert operation can ruin most of the savings here.
		// I had a concept where you could pick a procedural sequence (e.g. 1, 2, 4, 8, 16) and use some of the underlying assumptions to make these backtrack operations faster, but I don't think it would work out that well (based on intuition).

		// Start with InsertionSort::lazy_leftward_sort(data, before_first, after_last, [](long n){return 4*n;});
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

	std::vector<long> generalized_pratt(std::set<long> factors_list, long last_value, bool exponentiate = true)
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
		for (int guaranteed_value = 0; true; guaranteed_value += 1)
		{
			// Set cannot be modified while iterators are traversing, but we can avoid that altogether.
			long current_value = *std::next(unexponentiated.begin(), guaranteed_value);
			if (current_value > last_value)
			{
				break;
			}
			for (long factor : factors_list) // this includes 1 but I don't mind
			{
				unexponentiated.insert(current_value*factor);
			}
		}

		// For e.g. a Pratt<2,3> you want to square the sequence to 4,9
		// For higher powers, I don't think it's quite the same
		// E.g. for a Pratt<2,3,5> I think you don't want numbers in the same fashion. Hence this code.
		int power = factors_list.size() - 1;
		if (!exponentiate)
		{
			power = 1;
		}
		std::vector<long> exponentiated;
		for (int index = 0; true; index += 1)
		{
			long value = *std::next(unexponentiated.begin(), index);
			long result = 1;
			// kinda equivalent to pow(value, power);
			for (int lazy_power = 0; lazy_power < power; lazy_power += 1)
			{
				result *= value;
			}
			if (result > last_value)
			{
				break;
			}
			exponentiated.push_back(result);
		}

		return exponentiated;
	}

	std::vector<long> gap_sequence_generator(std::function<long(long)> lambda_function, long values)
	{
		std::vector<long> result;
		for (int value = 1; value <= values; value += 1)
		{
			result.push_back(lambda_function(value));
		}

		return result;
	}

	// Okay, yeah, Tokuda just adds a constant, but I'm mostly limited by how functions work.
	std::function<long(long, double, double)> generalized_tokuda = [](long n, double ratio, double skew) // simplicity seems to work
	{
		double value = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value = ratio * value + skew*iteration; // possible rationale for adding k on each iteration, every iteration costs one more (sort of like the "Two Egg Problem" with k eggs) so you have to account for the added cost on each step.
		}
		return ceil(value);
	};

	std::function<long(long)> ciura_extension = [](long n) // simplicity seems to work
	{
		// 1 4 10 25 60 140 324 742 1692 3850 8750 19875 45135 102486 232696 528321 1199504 2723342 6183028 14037821 31871158 72359549 164283441 372985289
		double ratio = 3*0.7567921710249769090317198; // three times the ratio of 2s and 3s via Euler's number: 2^(1-k)*3^(k) = e
		double skew = +1;
		return generalized_tokuda(n, ratio, skew);
	};

	std::function<long(long)> natural_extension = [](long n)
	{
		// Equivalent to 1 2 3 5 8 16 37 92 243 649 1755 4757 12919 35104 95408 259330 704914 1916137 5208581 14158372 38486424 104616926 284378265 773020247
		double ratio = 2.718281828459045235360287471352662497757247093699959574966; // it just feels natural
		double skew = -1;
		return generalized_tokuda(n, ratio, skew);
	};

	std::function<long(long)> invisal_extension = [](long n) // simplicity seems to work
	{
		// Equivalent to 1 3 7 16 39 95 236 584 1451 3604 8953 22242 55253 137263 340994 847116 2104452 5227995 12987672 32264690 80153719 199122281 494670533 1228887775
		double ratio = 2.484255063243666636247322; /*notably, 2.48 does way better*/ // (3*0.7567921710249769090317198)^.5*(e^.5) i.e. @ invisal's 2.48 sequence https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence
		double skew = 0;
		return generalized_tokuda(n, ratio, skew);
	};

	std::vector<long> triple_threat =
	{
		// Ciura extension (small size):   1 4 10 25 60 140 324 742 1692 3850 8750 19875 45135 102486 232696 528321 1199504 2723342 6183028  14037821 31871158 72359549  164283441 372985289
		// Invisal extension (med size):   1 3 7  16 39 95  236 584 1451 3604 8953 22242 55253 137263 340994 847116 2104452 5227995 12987672 32264690 80153719 199122281 494670533 1228887775
		// Natural extension (large size): 1 2 3  5  8  16  37  92  243  649  1755 4757  12919 35104  95408  259330 704914  1916137 5208581  14158372 38486424 104616926 284378265 773020247
		// First/best attempt:
		1, 4, 10, 25, 60, 140, 324, 742, 1692, 3850, // Ciura extension values
		8953, 22242, 55253, 137263, 340994, 847116, 2104452, 5227995, 12987672, 32264690, 80153719, 199122281, 494670533, 1228887775 // Invisal extension values
		// In theory the natural extension values come into play, but it takes a while
	};

	// Eventually I'll try Ciura only
	std::vector<long> triple_threat_primes =
	{
		1, 4, 10, 23, 59, 139, 317, 739, 1669, 3847, // Ciura extension values
		8951, 22229, 55249, 137251, 340979, 847109, 2104441, 5227991, 12987647, 32264689, 80153713, 199122233, 494670511, 1228887763 // Invisal extension values
		// In theory the natural extension values come into play, but it takes a while
	};

	// Hmmm, even this does worse than the 2.25 Ciura extension with primes. It's more proof that Tokuda stumbled on a really good ratio.
	std::vector<long> ciura_extension_primes =
	{
		// It's worth mentioning all of these sequences don't seem to have primes outside k > 5 or so, which is either a cool coincidence or proof they have some degree of value (since they use irrational numbers and it shouldn't be trivial to avoid primes with those).
		1, 4, 10, 23, 59, 139, 317, 739, 1669, 3847, 8747, 19867, 45131, 102481, 232681, 528317, 1199491, 2723341, 6183017, 14037809, 31871137, 72359537, 164283403, 372985267 // Ciura extension values
	};

	// I still haven't tried these with a floor to prime function, so I'm sure this could still work out.
	// It's also worth mentioning I am greedily choosing the largest gap sequence.
	// It's possible that it's better to start with the natural extension and towards Ciura for asymptotic performance (hard to say)
	// It's also possible that it's best to cycle through the sequences in some fashion (although it would be difficult).

	// Hmmm, there's a good idea.
	// Up until now I've tried "threading the needle" around a sequence (by marginal changes that are basically negligible asymptotically).
	// What if, instead, I do a sine curve of the three sequences?
	// First do ~2.27n + k, then ~2.48n + 0, then ~2.718n - k, then ~2.48n + 0, then ~2.27n + k, then ...
	// The basic idea: you use different "factorization highways" to get better statistical properties.
	// There's probably no need to use primes anymore if I do that.
	// This was an old concept, but I didn't know the full details of the functions when I previously thought about it.
	// These three functions make total sense; it's not unfounded to say ~2.25, ~2.48, and ~e should yield some good results.
	// You could use an actual sine curve (not a 4-cycle), and maybe that would yield better performance if you played around with the phaseshift and frequency (I think I got the amplitude and center, though)

	std::vector<long> ciura_extended_gap_sequence_attempt1 = // this seemed to work, I'm guessing it's mostly because of Ciura.
	{
		// Raw data:
		//1, 4, 10, 24, 58, 136, 311, 706, 1595, 3598, 8105, 18246, 41065, 92408, 207931, 467860, 1052700, 2368590, 5329346, 11991046, 25487633
		1, 4, 10,
		23, // 4+10+10 = 24 so try next smaller number
		57, //24 + 24 + 10 = 58 so try next smaller number
		132, // this doesn't follow the pattern I tried above
		301,
		701,
		1583, // lazily fetch next smallest prime
		3593,
		8101,
		18233,
		41057,
		92401,
		207931,
		467833
	};

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

	// TODO: variable primorial gap sequence using k#/i# (thus it e.g. sorts on 2, then 3, then 5 instead of 5 then 3 then 2)
	// primorial: 1, 2, 6, 30, 210, 2310, 30030, 510510,
	// "k#/i#": 510510/1, 510510/2, 510510/6, 510510/30, 510510/210, 510510/2310, 510510/30030, 510510/510510, (reverse ordered and only an example since it's variable)
	// upon thinking about it more, I feel like this wouldn't worth but I might as well try it if I have time.
	// Extending to other values that aren't arrays of size k#: 1) sort individual subarrays and merge or 2) double over with more primorials with remainder or 3) interpolate with standard gap sequences to get to primorial sorting

	// concept:
	// First generalized_pratt<2> number
	// Next generalized_pratt<2, 3> numbers
	// Next generalized_pratt<2, 3, 5> numbers
	// Next generalized_pratt<2, 3, 5, 7> numbers, etc
	// Next generalized_pratt<2, 3, 5, 7, 11> numbers, etc
	std::vector<long> probably_reliable =
	{
		1, 2,
		4, 9, 16,
		27, 64, 125, 216, 512,
		625, 1296, 2401, 4096, 6561, 10000,
		16807, 32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576,
		1771561, 2985984, 4826809, 7529536, 11390625, 16777216, 34012224,
		62748517
	};
	// NOTE: I think this is a serious candidate for a parallel time O(lg(n)) sort. The sorting network construction would probably be hellish, but I think it's worth investigating (even if it would require "recursion")
}