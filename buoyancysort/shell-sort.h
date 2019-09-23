#pragma once

#include <algorithm>
#include <climits>
#include <functional>
#include <iterator>
#include <math.h>
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

	// Another variant had slightly better performance (namely quarter rotations going through all four cases), but this matches Ciura's sequence better.
	std::function<long(long)> generalized_ciura = [](long n) // simplicity seems to work
	{
		const double lower_multiplier = 2.25; // irrational constant is the solution to 2^(1-k)*3^(k) = e
		const double upper_multiplier = 2.7; // e / Euler's constant
		const double pi = 3.141592653589793238462643383279502884197169399375105;
		
		const double phase_shift = 0;
		const double frequency = 0.618033989; // 1 - phi

		double value = 1;
		double phase = phase_shift;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			double similarity = cos(phase); // There's several ways this could work, but this one is the most intuitive
			double fraction = (similarity + 1) / 2;
			double multiplier = pow(lower_multiplier, fraction)*pow(upper_multiplier, 1 - fraction);
			value = multiplier * value + similarity * iteration; // possible rationale for adding k on each iteration, every iteration costs one more (sort of like the "Two Egg Problem" with k eggs) so you have to account for the added cost on each step.

			phase = std::fmod(phase + frequency, 2*pi/3); // move the multipliers in a sine curve.
		}
		return ceil(value);
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

	std::vector<long> interpolated_primorial =
	{
		1,
		2,
		4, 6,
		12, 18, 24, 30,
		60, 90, 120, 150, 180, 210,
		420, 630, 840, 1050, 1260, 1470, 1680, 1890, 2100, 2310,
		4620, 6930, 9240, 11550, 13860, 16170, 18480, 20790, 23100, 25410, 17720, 30030,
		60060, 90090, 120120, 150150, 180180, 210210, 240240, 270270, 300300, 330330, 360360, 390390, 420420, 450450, 480480, 510510,
		1021020, 1531530, 2042040, 2552550, 3063060, 3573570, 4084080, 4594590, 5105100, 5615610, 6126120, 6636630, 7147140, 7657650, 8168160, 8678670, 9189180, 9699690,
	};

	std::vector<long> interpolated_primorial2 =
	{
		1,
		2,
		4, 6,
		12, 18, 30,
		60, 90, 150, 210,
		420, 630, 1050, 1470, 2310,
		4620, 6930, 11550, 16170, 25410, 30030,
		60060, 90090, 150150, 210210, 330330, 390390, 510510,
		1021020, 1531530, 2552550, 3573570, 5615610, 6636630, 8678670, 9699690,
	};

	std::vector<long> interpolated_factorial =
	{
		1,
		2,
		4, 6,
		12, 18, 24,
		48, 72, 96, 120,
		240, 360, 480, 600, 720,
		1440, 2160, 2880, 3600, 4320, 5040,
		10080, 15120, 20160, 25200, 30240, 35280, 40320,
		80640, 120960, 161280, 201600, 241920, 282240, 322560, 362880,
		725760, 1088640, 1451520, 1814400, 2177280, 2540160, 2903040, 3265920, 3628800,
		7257600 // ... 39916800,
	};

	std::vector<long> interpolated_factorial2 =
	{
		1,
		2,
		4, 6,
		12, 18, 24,
		48, 72, 120,
		240, 360, 600, 720,
		1440, 2160, 3600, 5040,
		10080, 15120, 25200, 35280, 40320,
		80640, 120960, 201600, 282240, 362880,
		725760, 1088640, 1814400, 2540160, 3628800,
		7257600 // ... 39916800,
	};

	// Keep this, it might be useful (at least on a theoretical level)
	std::vector<long> interesting_idea =
	{
		/*
		1,
		2,
		6, 10, // 2*{3, 5}
		42, 66, // 6*{7, 11}
		130, 170, // 10*{13, 17}
		798, 966, // 42*{19, 23}
		1914, 2046, // 66*{29, 31}
		4810, 5330, // 130*{37, 41}
		7310, 7990, // 170*{43, 47}
		1914, 2046, // 798*{53, 59}
		*/

		// I think this is the foundation for the O(lg(n)) parallel time sorting algorithm. It is extremely fast with the modified "CombSort" (simulated sorting network)
		// It's possible if you sort within the largest prime factor -- bitonic sorter is fine since the subsort size doesn't grow that quickly (e.g. 127 for 1,014,730 and you sort four times then it might sort the data).

		// Yesh, got it to work with size 10,000,000 with the following stats:
		// Phases: 4
		//	2710540386
		//  271.054 // compared to lg(10,000,000)^2 or 540.725107 // I think a bitonic sorter would cost about 180, though =/ (although the performance would be better than 271 I think since a size 199 bitonic sorter would cost something like 36 (and you need the full cost 4 times over))
		// Notably, a bitonic sorter would be faster than a size 199 insertion sort
		// That being said, I don't know if the insertion-sort behavior of the comb sort function would work perfectly (I don't know how to properly construct sorting networks, especially for hardware).
		// I'm also concerned about the growth rate of the gaps in this sequence (e.g. 1,677 to 7,990 or 189,501 to 1,014,730 are sizeable gaps, and there's a good chance they grow unbounded). I could manually construct good sequences by hand but an algorithm would be nice.

		1,
		2, 3, // 1*{2, 3}
		10, 14, // 2*{5, 7}
		33, 39, // 3*{11, 13}
		170, 190, // 10*{17, 19}
		322, 406, // 14*{23, 29}
		1023, 1221, // 33*{31, 37}
		1599, 1677, // 39*{41, 43}
		7990, 9010, // 170*{47, 53}
		11210, 11590, // 190*{59, 61}
		21574, 22862, // 322*{67, 71}
		29638, 32074, // 406*{73, 79}
		84909, 91047, // 1023*{83, 89}
		118437, 123321, // 1221*{97, 101}
		164697, 171093, // 1599*{103, 107}
		182793, 189501, // 1677*{109, 113}
		1014730, 1046690, // 7990*{127, 131}
		1234370, 1252390, // 9010*{137, 139}
		1670290, 1692710, // 11210*{149, 151}
		1819630, 1889170, // 11590*{157, 163}
		3602858, 3732302, // 21574*{167,173}
		4092298, 4138022, // 22862*{179, 181}
		5660858, 5720134, // 29638*{191, 193}
		6318578, 6382726, // 32074*{197, 199}
		17915799, 18934707 // 84909*{211, 223}

		/* // possible superceding concept (values intentionally out of order to show how the recursion works, appears to be more-or-less the same)
		1,
		2, 3, // 1*{2, 3}
		10, 22, // 2*{5, 11}
		21, 39, // 3*{7, 13}
		170, 310, // 10*{17, 31}
		399, 777, // 21*{19, 37}
		506, 902, // 22*{23, 41}
		1131, 1677, // 39*{29, 43}
		*/

		/* // let's try manually?
		1,
		3, // 1*{3}
		6, // 3*{2}
		15, // 3*{5}
		42, // 6*{7}
		102, // 6*{17}
		195, // 15*{13}
		462, // 42*{11}
		798, // 42*{19}
		2346, // 102*{23}
		5655, // 195*{29}
		*/

		/* // another attempt mirroring Fibonacci
		1,
		2, // 2
		3, // 3
		10 // 2*{5}
		21, 33, // 3*{7,11}
		130, 170, 190, // 10*{13,17,19}
		, // 21*{23, 29, 31, 37, 41}
		*/
	};
}