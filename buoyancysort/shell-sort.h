#pragma once

#include <algorithm>
#include <assert.h>
#include <climits>
#include <functional>
#include <iterator>
#include <math.h>
#include <stddef.h>
#include <vector>
#include "sorted.h"

//typedef int current_type;
typedef IntThatTracksComparisons::IntThatTracksComparisons current_type;

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
		std::cout.precision(17);
		std::cout << value << std::endl;
		return ceil(value);
	};

	// Actually, this can't be swap-optimal, right? My first impression is that swap optimal should approximately be n-1, n-2, n-3, n-4... 3, 2, 1 (which isn't actually true, but I think there would be more gaps).
	std::function<long(long)> natural_extension = [](long n)
	{
		// Equivalent to 1 2 3 5 8 16 37 92 243 649 1755 4757 12919 35104 95408 259330 704914 1916137 5208581 14158372 38486424 104616926 284378265 773020247
		double ratio = 2.718281828459045235360287471352662497757247093699959574966; // it just feels natural
		double skew = -1;
		return generalized_tokuda(n, ratio, skew);
	};

	std::function<long(long)> invisal_extension1 = [](long n) // simplicity seems to work
	{
		// Equivalent to 
		double ratio = 2.48; // invisal's 2.48 sequence https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence
		double skew = 0; // solution to sqrt(5)^k*e^(1-k) = 2.48, except interpolated to range [-1, +1]
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

	// I've confirmed that value 9 was coincidentally close to 3530.
	std::function<long(long)> generalized_ciura3 = [](long n) // simplicity seems to work
	{
		// (-8 (9 + 5 n) (-1 + log(9/4)) + (9/4)^n (-122 + 144 log(3/2) + 25 log(5)))/(25 (-2 + log(5)))
		// You can think of this as either Ciura's sequence (or close to it) or an upgraded Tokuda sequence (either works, really).
		double ratio = 2.25;
		double skew = +0.971108; // solution to sqrt(5)^k*e^(1-k) = 2.25 // I just realized: is it luck that this worked out? XD Technically, I've been trying to map onto the range [-1, +1], I am accidentally mapping onto [0, 1] here. // Oh wait, I fucked up even harder than I originally thought: I used geometric averaging despite the strong evidence that normal averaging was relevant here i.e. sqrt(5)*(k) + e*(1-k), k= ?... that being said, I don't actually have a method of finding k here, so I guess geometric averaging is still the way to go when you don't know how to interpolate (you just have to be careful because Wolfram Alpha always assumes positive I think). You probably need to exchange the k and 1-k in the equation when this happens.
		return generalized_tokuda(n, ratio, skew);
	};

	// long overdue
	void empirical_comparison(std::vector<long> gap_sequence1, std::vector<long> gap_sequence2, long lower_size, long upper_size)
	{
		// A relevant note: I don't have to worry about traditional programming benchmarking techniques, since counting comparisons is an "objective" benchmark of a deterministic algorithm (as opposed to CPU timing / caching / etc which would be "subjective").
		std::vector<current_type> data1(upper_size);
		std::vector<current_type> data2(upper_size);

		std::random_device random_device;
		std::mt19937 random_number_generator(random_device());
		std::uniform_int_distribution<long> size_distribution(lower_size, upper_size);

		long long comparison_count1 = 0;
		long long comparison_count2 = 0;

		const int tests = 41; // usually statistics start becoming reliable in the 20-50 range, and generally you want odd numbers for tie-breakers.
		for (int test = 0; test < tests; test += 1)
		{
			// Boilerplate-ish =(, I can do a foreach but I don't think it helps with readability.
			long size = size_distribution(random_number_generator);
			for (long i = 0; i < size; i += 1)
			{
				data1[i] = size - i;
			}
			random_number_generator.seed(test);
			std::shuffle(&data1[0], (&data1[size - 1]) + 1, random_number_generator);
			for (long i = 0; i < size; i += 1)
			{
				data2[i] = data1[i]; // if it's not a copy, it's not a 1:1 comparison. I noticed one off-by-one and one completely incorrect algorithm in my prior (uncompiled) attempts, which is why I deem this safer.
			}

			IntThatTracksComparisons::reset_comparisons();
			ShellSort::sort<current_type>((current_type*)data1.data(), -1, size, gap_sequence1);
			long long comparisons = IntThatTracksComparisons::get_comparisons();
			comparison_count1 += comparisons;
			assert(Sorted::verify((current_type*)data1.data(), -1, size));
			IntThatTracksComparisons::reset_comparisons();
			ShellSort::sort<current_type>((current_type*)data2.data(), -1, size, gap_sequence2);
			comparisons = IntThatTracksComparisons::get_comparisons();
			comparison_count2 += comparisons;
			assert(Sorted::verify((current_type*)data2.data(), -1, size));
		}
		std::cout << comparison_count1 << ":" << comparison_count2 << " (" << (comparison_count2 - comparison_count1) << ")";
	}

	std::vector<long> ciura_gap_sequence =
	{
		1, 4, 10, 23, 57, 132, 301, 701,
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
		//1577, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711
	};

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

	std::vector<long> octavarium8 =
	{
		// g(n) = 12/5 * g(n-1) + k*n, g(0) = 1
		// 1, 3/4, 6/7, 15/16, 38/39, 94/95, 226/227, 545/546, 1310/1311, 3145/3146, 7551/7552, 18125/18126, 43503/43504, 104411/104412, 250588/250589, 601415/601416
		1, 3, 7, 16, 38, 94, 227, 546, 1311, 3145, 7552, 18126, 43504, 104411, 250589, 601415
		//?, down/0, up/1, up/1, down/0, down/0,  // gonna try Apéry's constant: zeta(3) thus .0011.0011.1011.1010
		// g(n) = (5^(-n) (12^n (-218 + 49 log(3/2) + 60 log(144/25)) - 2 5^(n + 1) (7 n + 12) (log(12/5) - 1)))/(49 (log(3/2) - 2))
	};

	std::vector<long> octavarium9 =
	{
		// g(n) = 12 / 5 * g(n - 1) + ((-1 + log(12 / 5)) / (-1 + log(zeta(3))))*n, g(0) = 1
		// 1, 3/4, 6/7, 15/16, 38/39, 93/94, 225/226, 543/544, 1305/1306, 3134/3135, 7523/7524, 18059/18060, 43343/43344, 104026/104027, 249666/249667, 599202/599203
		1, 3, 7, 16, 38, 93, 226, 544, 1306, 3134, 7524, 18060, 43344, 104026, 249667, 599202
	};

	std::vector<long> eleven_squared_seven_squared = // I believe there is a possibility that the 3^2/2^2 and 11^2/7^2 equations have k values that sum to 1 (complementary).
	{
		//?011.0000.
		// possibly inverse of golden ratio: 1.1001111000110111011
		// thus 0011.0000.1110.0100 (although this is super arbitrary, I think irrational rounding is probably better than alternating rounding, even though it would be better to just test all pairs)
		// g(n) = 11^2/7^2 * g(n-1) + (1 - 0.9716) * n, g(0) = 1
		// 1, 3/4, 6/7, 15/16, 38/39, 94/95, 233/234, 577/578, 1427/1428, 3525/3526, 8704/8705, 21496/21497, 53082/53083, 131081/131082, 323691/323692, 799321/799322
		1, 3, 7, 16, 38, 94, 233, 577, 1428, 3526, 8705, 21496, 53082, 131082, 323691, 799321
	};

	std::vector<long> extrapolated_ciura_tokuda =
	{
		// #Rekt (did not beat Ciura at all, for the billionth time) -- 1, 4, 10, 23, 57, 132, 305, 692 (and 1, 3/4, 9/10, 23/24, 56/57, 131/132, 302/303, 687/688)
		// g(n) = 3^2/2^2 * g(n-1) + (1 - e^(-pi)) * n, g(0) = 1
		// 1, 3/4, 9/10, 23/24, 56/57, 131/132, 302/303, 687/688
		1, 4, 10, 23, 57, 132, 
	};


	// concept:
	// First generalized_pratt<2> number
	// Next generalized_pratt<2, 3> numbers
	// Next generalized_pratt<2, 3, 5> numbers
	// Next generalized_pratt<2, 3, 5, 7> numbers, etc
	// Next generalized_pratt<2, 3, 5, 7, 11> numbers, etc
	std::vector<long> probably_reliable =
	{
		1, //2,
		4, 9, 16,
		27, 64, 125, 216, 512,
		625, 1296, 2401, 4096, 6561, 10000,
		16807, 32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576,
		1771561, 2985984, 4826809, 7529536, 11390625, 16777216, 34012224,
		62748517
	};
	// NOTE: I think this is a serious candidate for a parallel time O(lg(n)) sort. The sorting network construction would probably be hellish, but I think it's worth investigating (even if it would require "recursion")

	// Keep this, it might be useful (at least on a theoretical level)
	std::vector<long> probably_reliable2 = // sprawling primes
	{
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
	};

	std::vector<long> probably_reliable3 = // sprawling primes (interesting property: not purely ascending order)
	{
		1,
		4, // 1*2^2
		9, // 1*3^2
		20, // 4*5
		63, // 9*7
		44, // 4*11
		117, // 9*13
		340, // 20*17
		1197, // 63*19
		460, // 20*23
		1827, // 63*29
		1364, // 44*31
		4329, // 117*37
		1804, // 44*41
		5031, // 117*43
		15980, // 340*47
		63441, // 1197*53
		20060, // 340*59
		73017, // 1197*61
		30820, // 460*67
		129717, // 1827*71
		33580, // 460*73
		144333, // 1827*79
		113212, // 1364*83
		385281, // 4329*89
		132308, // 1364*97
		437229, // 4329*101
		185812, // 1804*103
		538317, // 5031*107
		196636, // 1804*109
		568503, // 5031*113
		2029460, // 15980*127
		8310771, // 63441*131
		2189260, // 15980*137
		8818299, // 63441*139
		2988940, // 20060*149
		11025567, // 73017*151
		3149420, // 20060*157
		11901771 // 73017*163
		// TODO: because of the nature of this function, you cannot efficiently sort 10,000,000 elements yet (you need more gap sequence numbers)
	};
}