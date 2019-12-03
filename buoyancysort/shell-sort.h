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

    const double e = 2.718281828459045235360287471352662497757247093699959574966;
	const double pi = 3.141592653589793238462643383279502884197169399375105;

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
			value = ratio * value + skew * iteration; // possible rationale for adding k on each iteration, every iteration costs one more (sort of like the "Two Egg Problem" with k eggs) so you have to account for the added cost on each step.
		}
		std::cout.precision(17);
		std::cout << value << std::endl;
		return ceil(value);
	};

	// Actually, this can't be swap-optimal, right? My first impression is that swap optimal should approximately be n-1, n-2, n-3, n-4... 3, 2, 1 (which isn't actually true, but I think there would be more gaps).
	std::function<long(long)> natural_extension = [](long n)
	{
		// Equivalent to 1 2 3 5 8 16 37 92 243 649 1755 4757 12919 35104 95408 259330 704914 1916137 5208581 14158372 38486424 104616926 284378265 773020247
		double ratio = e; // it just feels natural
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

			phase = std::fmod(phase + frequency, 2 * pi / 3); // move the multipliers in a sine curve.
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

	std::function<long(long)> generalized_ciura4 = [](long n) // simplicity seems to work
	{
		// You can think of this as either Ciura's sequence (or close to it) or an upgraded Tokuda sequence (either works, really).
		double ratio = 5 - e;
		double skew = e + pi - 5;
		return generalized_tokuda(n, ratio, skew);
	};


	//std::function<long(long)> extrapolated_invisal_hybrid = [](long n) // g(k) = (e - 1/5) * g(k-1) + zeta(-1) * k, g(3) = 32.8844 (first few values from g(k) = (5 - e) * g(k-1) + zeta(2) * k, g(0) = 1)
	//{
	//	return generalized_tokuda(n, ratio, skew); // varying parameters
	//};

	std::function<long(long)> extrapolated_invisal = [](long n) // g(k) = (e - 1/5) * g(k-1) + zeta(-1) * k, g(0) = 1
	{
		double ratio = e - 1./5;
		double skew = -1./12;
		return generalized_tokuda(n, ratio, skew);
	};

	std::function<long(long)> extrapolated_ciura = [](long n) // g(k) = (5 - e) * g(k-1) + zeta(2) * k, g(0) = 1
	{
		double ratio = 5 - e;
		double skew = +pi*pi/6;
		return generalized_tokuda(n, ratio, skew);
	};

	// long overdue
	void empirical_comparison(std::vector<long> gap_sequence1, std::vector<long> gap_sequence2, long lower_size, long upper_size, int tests = 41)
	{
		// A relevant note: I don't have to worry about traditional programming benchmarking techniques, since counting comparisons is an "objective" benchmark of a deterministic algorithm (as opposed to CPU timing / caching / etc which would be "subjective").
		std::vector<current_type> data1(upper_size);
		std::vector<current_type> data2(upper_size);

		std::random_device random_device;
		std::mt19937 random_number_generator(random_device());
		std::uniform_int_distribution<long> size_distribution(lower_size, upper_size);

		long long comparison_count1 = 0;
		long long comparison_count2 = 0;

		// tests should really depend on the array size (smaller arrays means more tests)
		//const int tests = 41; // usually statistics start becoming reliable in the 20-50 range, and generally you want odd numbers for tie-breakers.
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

	std::vector<long> invisal_gap_sequence = gap_sequence_generator([](int n) { return ceil(pow(2.48, n-1)); }, 20);

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
		1, 4, 10, 23, 57, 132, 301, 701, 1607, 3672, 8387, 19142, 43688, 99692, 227481, 519058, 1184359, 2702385, 6166098, 14069311
	};

	std::vector<long> extrapolated_ciura_tokuda4 =
	{
		1, 4, 10, 23, 57, 142, 361, 925, 2385, 6160, 15924, 41176, 106493, 275442, 712445
		// 1, 3, 7, 16, 37, 93, 234, 560, 1410, 115004129:117882077 (2877948)
		// 1, 3, 8, 17, 37, 93, 234, 560, 1410, 115041398:116540410 (1499012)
		// 1, 3, 8, 17, 37, 93, 235, 597, 1517
	};

	std::vector<long> extrapolated_hybrid_invisal =
	{
		1, 4, 12, 33, 83, 208, 522, 1313, 3306, 8323, 20958, 52777, 132905, 334691, 842845
	};

	std::vector<long> extrapolated_ciura_tokuda2 =
	{
		1, 4, 13, 33, 82, 195, 454, 1047, 2401, 5493, 12549, 28651, 65392, 149226, 340515, 776983, 1772881, 4045243, 9230133, 21060592,
	};

	std::vector<long> extrapolated_ciura_tokuda3 =
	{
		1, 4, 10, 23, 57, 132, 301, 758, 1909, 4805, 12099, 30467, 76723, 193208, 486550
	};

	std::vector<long> extrapolated_invisal2 = 
	{
		1, 3, 6, 15, 37, 93, 233, 585, 1471, 3704, 9325, 23481, 59130, 148904, 374980, 944304, 2378021, 5988526, 15080793, 37977685
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

	std::vector<long> probably_reliable4 = gap_sequence_generator([](int n) { return 11 * pow((1.25f), n) - 2 * (5 + n); }, 70);
	std::vector<long> probably_reliable5 = gap_sequence_generator([](int n) { return pow((1.25f), n); }, 96);

	//g(n) = (5 / 4) * g(n-1) + .5 * n, g(5) = 132 thus g(n) = 19 2^(13 - 2 n) 5^(-5 + n) - 2 (5 + n)
	//std::vector<long> probably_reliable6 = gap_sequence_generator([](int n) { return 19 * pow(2, (13 - 2 * n)) * pow(5, (-5 + n)) - 2 * (5 + n); }, 70);
	std::vector<long> probably_reliable6 = 
	{
		1, 4, 10, 23, 57, 132, // up to this point, just use Ciura
		168, 213, 270, 343, 433, 547, 690, 869, 1094, 1375,
		1727, 2167, 2718, 3408, 4270, 5348, 6696, 8381, 10489, 13123,
		16417, 20535, 25683, 32119, 40164, 50220, 62791, 78506, 98149, 122704,
		153398, 191766, 239727, 299679, 374618, 468294, 585388, 731757, 914718, 1143420,
		1429299, 1786647, 2233333, 2791691, 3489638, 4362074, 5452618, 6815799, 8519776, 10649748,
		13312213, 16640295, 20800398, 26000527, 32500688, 40625891, 50782395, 63478025, 79347564, 99184487,
		123980642, 154975837, 193719830, 242149822, 302687313
	};

	std::vector<long> probably_reliable7 = gap_sequence_generator([](int n) { return (-566858243176225 + 623400412190497 * pow((26615281.0 / 21298225), n) - 113243855025600 * n) / 56542169014272; }, 70);
	//std::vector<long> probably_reliable8 = gap_sequence_generator([](int n) { return (pow(21298225.0, -n) * (pow(-21298225.0, 1 + n) + pow(26615281.0, 1 + n))) / 5317056; }, 70);
	//std::vector<long> probably_reliable8 = gap_sequence_generator([](int n) { return -4.005642408129612 + 5.005642408129612 * exp(0.22286178861539475 * n); }, 70);
	std::vector<long> probably_reliable8 =
	{
		1, 2, 3, 5, 8, 11, 15, 19, 25, 33,
		42, 54, 68, 86, 109, 137, 173, 217, 272, 341,
		427, 535, 670, 838, 1048, 1311, 1640, 2050, 2563, 3204,
		4005, 5006, 6257, 7820, 9773, 12214, 15265, 19077, 23840, 29793,
		37232, 46528, 58144, 72661, 90802, 113471, 141800, 177202, 221441, 276724,
		345809, 432141, 540025, 674842, 843316, 1053849, 1316941, 1645714, 2056564, 2569982,
		3211574, 4013337, 5015260, 6267310, 7831931, 9787157, 12230501, 15283820, 19099394, 23867517, 29825993
	};

	//std::vector<long> probably_reliable9 = gap_sequence_generator([](int n) { return -4 + 5 * pow(1.25, n); }, 70);
	std::vector<long> probably_reliable9 =
	{
		1, 2, 3, 5, 8, 11, 15, 19, 25, 33,
		42, 54, 68, 86, 109, 138, 173, 218, 273, 342,
		429, 538, 673, 843, 1054, 1319, 1650, 2063, 2580, 3227,
		4034, 5044, 6306, 7884, 9856, 12321, 15403, 19255, 24070, 30088,
		37611, 47015, 58770, 73464, 91831, 114790, 143488, 179362, 224203, 280255,
		350320, 437901, 547378, 684223, 855280, 1069101, 1336378, 1670473, 2088093, 2610117,
		3262648, 4078311, 5097890, 6372363, 7965455, 9956820, 12446026, 15557534, 19446918, 24308649, 30385812
	};

	// g(n) = (1.25) * g(n-1) - 1, g(2) = 9
	// 4 + 4^(2 - n) 5^(-1 + n)
	//std::vector<long> probably_reliable10 = gap_sequence_generator([](int n) { return 4 + pow(4, (2 - n)) * pow(5, (-1 + n)); }, 70);
	std::vector<long> probably_reliable10 =
	{
		1, 4,
		9, 10, 11, 13, 16, 19, 23, 27,
		33, 41, 50, 62, 76, 94, 117, 146, 181, 226,
		281, 350, 437, 546, 681, 851, 1062, 1327, 1658, 2071,
		2588, 3235, 4042, 5052, 6314, 7892, 9864, 12329, 15411, 19263,
		24078, 30096, 37619, 47023, 58778, 73472, 91839, 114798, 143496, 179370,
		224211, 280263, 350328, 437909, 547386, 684231, 855288, 1069109, 1336386, 1670481,
		2088101, 2610125, 3262656, 4078319, 5097898, 6372371, 7965463, 9956828, 12446034, 15557542, 19446926,
	};

	// None of the attempts thus far have had low variance in comparisons, which suggests my theory you could make an O(nlgn) worst case ShellSort is incorrect (a proof already exists for this, so it was a long-shot).
	// This does remind me: if you can't get an O(nlgn) worst case ShellSort here, then you definitely can't do it in a sorting network. You could still maybe get a O(n*(lgn/lglgn)^2) sorting network.

	// g(n) = 1.05 * g(n-1) - 1, g(2) = 23
	// 20 + (20/7)^(2 - n) 3^(-1 + n)
	//std::vector<long> probably_reliable11 = gap_sequence_generator([](int n) { return 20 + pow(20.0/7, 2-n) * pow(3, -1 + n); }, 300);
	std::vector<long> probably_reliable11 = 
	{
		1, 4, 10,
		23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 48, 49, 51,
		52, 54, 56, 57, 59, 61, 63, 66, 68, 70,
		73, 76, 78, 81, 84, 88, 91, 95, 98, 102,
		106, 111, 115, 120, 125, 130, 136, 142, 148, 154,
		161, 168, 176, 183, 192, 200, 209, 219, 229, 239,
		250, 262, 274, 287, 300, 314, 329, 344, 360, 377,
		395, 414, 434, 454, 476, 499, 523, 548, 575, 602,
		632, 662, 694, 728, 763, 801, 840, 881, 924, 969,
		1016, 1066, 1119, 1174, 1231, 1292, 1355, 1422, 1492, 1566,
		1643, 1725, 1810, 1899, 1993, 2092, 2196, 2304, 2419, 2539,
		2665, 2797, 2936, 3081, 3235, 3395, 3564, 3741, 3927, 4123,
		4328, 4543, 4770, 5007, 5257, 5518, 5793, 6082, 6385, 6703,
		7038, 7389, 7757, 8144, 8550, 8977, 9424, 9895, 10388, 10907,
		11451, 12023, 12623, 13253, 13915, 14610, 15339, 16105, 16909, 17754,
		18641, 19572, 20549, 21576, 22654, 23785, 24974, 26221, 27531, 28907,
		30351, 31868, 33460, 35132, 36888, 38731, 40667, 42699, 44833, 47074,
		49427, 51897, 54491, 57215, 60074, 63077, 66230, 69541, 73017, 76667,
		80499, 84523, 88748, 93184, 97843, 102734, 107870, 113262, 118924, 124869,
		131112, 137667, 144549, 151775, 159363, 167330, 175696, 184480, 193703, 203387,
		213555, 224232, 235443, 247214, 259574, 272551, 286178, 300486, 315509, 331284,
		347847, 365238, 383499, 402673, 422806, 443945, 466141, 489448, 513919, 539614,
		566594, 594922, 624667, 655900, 688694, 723128, 759283, 797246, 837107, 878962,
		922909, 969053, 1017505, 1068379, 1121797, 1177886, 1236780, 1298618, 1363548, 1431724,
		1503309, 1578474, 1657396, 1740265, 1827278, 1918641, 2014572, 2115299, 2221063, 2332115,
		2448720, 2571155, 2699712, 2834697, 2976430, 3125251, 3281513, 3445587, 3617866, 3798758,
		3988695, 4188129, 4397534, 4617410, 4848279, 5090692, 5345226, 5612486, 5893110, 6187764
	};

	// g(n) = (72^2/43^2) * g(n-1) - (1-e^(-pi)), g(0) = 1
	// (1849^(-n) e^(-pi) (1849 (-1849^n + 5184^n) + (743 2^(1 + 6 n) 81^n + 1849^(1 + n)) e^pi))/3335
	//std::vector<long> probably_reliable12 = gap_sequence_generator([](int n) { return ceil((pow(1849, -n) * exp(-3.1415926) * (1849 * (pow(-1849, n) + pow(5184, n)) + (743 * pow(2, (1 + 6 * n)) * pow(81, n) + pow(1849, (1 + n)) * exp(3.1415926)))) / 3335); }, 20);
	// there's an error when I added parenthasis.
	std::vector<long> probably_reliable12 =
	{
		1, 4, 9, 22, 60, 166, 464, 1298, 3637, 10195,
		28583, 80135, 224671, 629903, 1766043, 4951414, 13882167, 38921119
	};

	// g(n) = (72^2/43^2) * g(n-1) - (1-e^(-pi)), g(0) = 1
	// (pow(1849, (-n)) * exp(-3.1415926) * (1849 * (pow(-1849, n) + pow(5184, n)) + (743 * pow(2,(1 + 6 * n)) * pow(81, n) + pow(1849^(1 + n))) * exp(3.1415926)))/3335
	//std::vector<long> probably_reliable13 = gap_sequence_generator([](int n) { return floor((pow(1849, (-n)) * exp(-3.1415926) * (1849 * (pow(-1849, n) + pow(5184, n)) + (743 * pow(2, (1 + 6 * n)) * pow(81, n) + pow(1849, (1 + n))) * exp(3.1415926))) / 3335); }, 20);
	std::vector<long> probably_reliable13 = 
	{
		1, 4, 10, 29, 81, 228, 639, 1793, 5026, 14091,
		39507, 110766, 310552, 870687, 2441126, 6844129, 19188732, 53799020, 150835109, 422893027
	};

	// g(n) = (11^2/7^2) * g(n-1) + 1, g(0) = 1
	// 1/72 (-49 + 49^(-n) 121^(1 + n))
	//std::vector<long> probably_reliable14 = gap_sequence_generator([](int n) { return ceil(1.0/72 * (-49 + pow(49,(-n)) * pow(121,(1 + n)))); }, 20);
	std::vector<long> probably_reliable14 =
	{
		1, 4, 10, 25, 62, 154, 381, 941, 2323, 5738,
		14169, 34989, 86402, 213360, 526869, 1301045, 3212784, 7933609, 19591157, 48378165,
		119464448
	};

	// g(n) = (pi/2)^2 * g(n-1) + (e^(-pi)), g(0) = 1
	// (4^(-n) e^(-pi) (-4^(1 + n) + pi^(2 n) (4 + e^pi (-4 + pi^2))))/(-4 + pi^2)
	// ceil((pow(4,(-n)) * exp((-pi)) * (pow(-4, (1 + n)) + pow(pi, (2 n)) * (4 + exp(pi) * (-4 + pow(pi, 2)))))/(-4 + pow(pi, 2)))
	//std::vector<long> probably_reliable15 = gap_sequence_generator([](int n) { return ceil((pow(4,(-n)) * exp((-3.1415926)) * (pow(-4, (1 + n)) + pow(3.1415926, (2 * n)) * (4 + exp(3.1415926) * (-4 + pow(3.1415926, 2)))))/(-4 + pow(3.1415926, 2))); }, 20);
	std::vector<long> probably_reliable15 =
	{
		// 1, 3, 7, 16, 39, 95, 233, 574, 1415, 3490, 8610, 21245, 52419, 129337, 319126, 787411, 1942857, 4793807, 11828244, 29185021, 72011150 (ceiled)
		1, 3, 7, 16, 38, 94, 233, 574, 1415, 3490,
		8610, 21245, 52419, 129337, 319126, 787411, 1942857, 4793807, 11828244, 29185021,
		72011150
	};
}