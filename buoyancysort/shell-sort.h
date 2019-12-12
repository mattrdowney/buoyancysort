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
		for (long gap : gap_sequence2)
		{
			std::cout << gap << ", ";
		}

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
		std::cout << comparison_count1 << ":" << comparison_count2 << " (" << (comparison_count2 - comparison_count1) << ")" << std::endl;
	}

	void empirical_comparison_range(std::vector<long> gap_sequence1, std::vector<long> gap_sequence2, long lower_size, long upper_size, long first, long last, int tests)
	{
		for (long extension_value = first; extension_value <= last; extension_value += 1)
		{
			std::vector<long> gap_sequence2_0 = gap_sequence2;
			gap_sequence2_0.push_back(extension_value);
			empirical_comparison(gap_sequence1, gap_sequence2_0, lower_size, upper_size, tests);
		}
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

	std::vector<long> extrapolated_ciura_tokuda1 = // Conjecture: g(k)^(1/k) = e (2.71828...) as k approaches infinity -- actually, I'm pretty sure this can't be true unless the multiplier approaches e (which seems incorrect, but asymptotics are often slow)
	{
		1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15870, 41042
	};

	std::vector<long> extrapolated_ciura_tokuda2 =
	{
		1, 4, 10, 23, 57, 142, 361, 925, 2383, 6150, 15870, 41042
	};

	// Reasonably (but by no means exhaustive) testing. Varying only one parameter at a time has limited assurances of correctness.
	// (First set of) Retests: 4 (3-5), 10 (9-13), 23 (21-31), 57 (47-73), 142 (132-148), 361 (355-368)
	// This gives me a little empirical confidence in the gap sequence values I intuitively believed to be true (minus the 1, 5, 17/19, 41 sequence tangent).
	// Next, I will test the later values.
	// via ShellSort::empirical_comparison(gap_sequence1, gap_sequence2, 45000, 120000, 41);
	// Second set of retests: 361 (355-368), 925 (917-935)
	// TODO: Review http://oeis.org/A221313
}