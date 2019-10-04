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

	// ~100% chance this was a coincidence, but I still feel it could be useful.
	std::function<long(long)> generalized_ciura3 = [](long n) // simplicity seems to work
	{
		// You can think of this as either Ciura's sequence (or close to it) or an upgraded Tokuda sequence (either works, really).
		double ratio = 2.25;
		double skew = +0.971108; // solution to sqrt(5)^k*e^(1-k) = 2.25 // I just realized: is it luck that this worked out? XD Technically, I've been trying to map onto the range [-1, +1], I am accidentally mapping onto [0, 1] here. // Oh wait, I fucked up even harder than I originally thought: I used geometric averaging despite the strong evidence that normal averaging was relevant here i.e. sqrt(5)*(k) + e*(1-k), k= ?... that being said, I don't actually have a method of finding k here, so I guess geometric averaging is still the way to go when you don't know how to interpolate (you just have to be careful because Wolfram Alpha always assumes positive I think). You probably need to exchange the k and 1-k in the equation when this happens.
		return generalized_tokuda(n, ratio, skew);
	};

	std::vector<long> ciura_gap_sequence =
	{
		1, 4, 10, 23, 57, 132, 301, 701,
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
		1577, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711
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

	// Yes, this gap sequence is good, but we might be able to do better.
	// There's two competing explanations (in my mind) for the @invisal 2.48 gap sequence ( https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence )
	// Explanation 1: it has something to do with 11 & 7 (which could actually imply 3 and 5 is the ideal gap sequence) -- it's possible you have to connect these sequences in-order (rather than starting with the "optimal"-ish sequence).
	// Explanation 2: 2.48 is halfway between e and something else (e.g. 2.25 or sqrt(5)) and is essentially an ideal center. This could mean you want to middle around that point or it could mean you want to fluctuate around that point.
	// Explanation ?: (There are plenty more reasons to think about)

	// I am particularly interested in the 3/5 "factorization highway" (notation abuse since I would normally call the 2/3 highway 1-3530). Based on Median-Of-Medians and other circumstantial evidence, I think 3 and 5 are some of the more important numbers in mathematics (at least from an information theory standpoint). 2 is important in an even/odd function sense (so perhaps it is more important than both combined) but I don't think about it as much.
	// All wild conjecture from here on: (and beforehand, really)
	// 5^2/3^2 = 2.77777778
	// in the 2/3 "factorization highway" you use a skew k that is the solution to the equation: sqrt(5)^k*e^(1-k) = 2.25
	// here that could mean: sqrt(3+5)^k*e^(1-k) = (5^2)/(3^2) thus k= 0.545086 OR sqrt(3*5-1)^k*e^(1-k) = (5^2)/(3^2) thus k = 0.067759953696224392398... (I think the second is more likely)
	// The binary representation of sqrt(3) would be used starting from the 4th digit or so.
	// The number would become a perfect integer on the 5^2 + 1 number.

	// Slight revision: I think that if the sqrt(2) binary is relevant, you start right after the decimal but the first one is ignored if you are starting from e.g. 1 or an integer.
	// Thus g(n) = (5^2)/(3^2) * g(n-1) + 0.067759953696224392398*n, g(0) = 1
	// Thus:
	// -0.059554646803322220 + 1.0595546468033222 e^(1.0216512475319813664 n) - 0.038114973954126221 n
	// 1, 2, 9, 23, 63, 174, 487, 1352, 3755, 10433, 28980, 80499, 223610, 621142, 1725394,
	
	std::vector<long> three_five =
	{
		1, 2, 9, 23, 63, 174, 487, 1352, 3755, 10433, 28980, 80499, 223610, 621142, 1725394
	};

	std::vector<long> three_five_ish = // it's not doing bad, but it isn't better. One issue: it's hard to verify by checking g(25) due to floating-point imprecision // The underlying problem could be a malformed equation or e.g. an unsolvable issue like 5^2/3^2 exceeds e, so the formula is less reliable.
	{
		1, 4, 10, 23, 63, 174, 487, 1352, 3755, 10433, 28980, 80499, 223610, 621142, 1725394
	};

	// I'm going to ignore 5^2/3^2 and focus on this octave / music theory angle.
	// I like the idea of using 23/2^3. Sure it exceeds e, but it looks pretty perfect to me.
	// 2.875 ratio. Interestingly, it relates to 2.56 (approximately the center of 2.25 and 2.875), which I also thought might be relevant because 2^6/5^2 (which I came up with because of the @invisal sequence)
	// Thus sqrt(23*8-1)^k*e^(1-k) = 23/2^3 means k ~= 0.034929375968604340232677412848592 (if that equation was actually the right one to use)
	// I'm just going to use intuition to round everything.

	std::vector<long> octavarium = // inspired by Dream Theater's song on the album of the same name.
	{
		// 1, 2/3, 8/9, 24/25, 70/71, 201/202, 580/581, 1669/1670, 4800/4801, 13802/13803, 39683/4, 114089/114090, 328008/9, 943025/6, 
		1, 3, 8, 25, 70, 202, 580, 1670, 4800, 13803, 39683, 114090, 328008, 943026
	};

	std::vector<long> octavarium2 = // inspired by Dream Theater's song on the album of the same name.
	{
		// 173/64
		// sqrt(173*64-1)^k*e^(1-k) = 173/64 thus k = -0.00152938302945376307735815324
		// g(n) = 173/64 * g(n-1) + -0.00152938302945376307735815324*n, g(0) = 1
		// 1, 2/3, 7/8, 19/20, 53/54, 144/145, 389/390, 1053/1054, 2846/2847, 7694/5, 20798/9, 56222/3, 151975/6, 410809/410810
		1, 3, 8, 20, 53, 144, 389, 1053, 2846, 7695, 20798, 56223, 151975, 410810
	};

	std::vector<long> octavarium3 = // inspired by Dream Theater's song on the album of the same name.
	{
		// 359/2^7
		// sqrt(359*2^7-1)^k*e^(1-k) = 359/2^7 thus k = 0.0071644966771190102429781047400...
		// g(n) = 359/2^7 * g(n-1) + 0.0071644966771190102429781047400*n, g(0) = 1
		// 1, 2/3, 7/8, 22/3, 62/3, 174/5, 489/90, 1373/4, 3852/3, 10805/6, 30305/6, 84996/7, 238388/9, 668604/5 
		1, 3, 8, 23, 63, 174, 490, 1374, 3852, 10805, 30306, 84996, 238388, 668605
	};

	// All of these are so in-elegant compared to, say, 2.4 or 12/5. Notably keyboards have 7 white keys and 5 black keys per octave.

	std::vector<long> octavarium4 = // inspired by Dream Theater's song on the album of the same name.
	{
		// 12/5
		// sqrt(sqrt(12)+sqrt(5))^k*e^(1-k) = 12/5 thus k = 0.959763455291092157165207366...
		// g(n) = 12/5 * g(n-1) + 0.959763455291092157165207366*n, g(0) = 1
		// 1, 3/4, 9/10, 26/27, 68/69, 168/169, 410/411, 991/992, 2387/2388, 5739/5740, 13783/13784, 33091/33092, 79430/79431, 190645/190646, 457562/457563
		1, 4, 10, 27, 69, 168, 410, 992, 2388, 5739, 13783, 33092, 79431, 190645, 457562
	};

	std::vector<long> octavarium5 = // going to try to mirror the 2.48 sequence, by guessing k and seeing if the sequence approximates the @invisal sequence
	{
		// I tried to reverse engineer k using the @invisal sequence
		// 12/5
		// k = ? (e.g. ~.15) // I checked what n would be via inferrence after using an approximate k to solve for n, and I think 0.156198 is a candidate: sqrt(1.5)^k*e^(1-k) = 12/5
		// g(n) = 12/5 * g(n-1) + k*n, g(0) = 1
		// 1, 3/4, 6/7, 15/16, 38/39, 94/95, 226/227, 545/546, 1310/1311, 3145/3146, 7551/7552, 18125/18126, 43503/43504, 104411/104412, 250588/250589, 601415/601416
		1, 3, 7, 16, 38, 94, 227, 546, 1310, 3145, 7552, 18126, 43503, 104411, 250589, 601416
		// Holy shit I wasn't looking: look at this recurrence solution:
		// 1/224 5^(-5 - n) (208471 3^n 4^(1 + n) - 11157 5^n (12 + 7 n))
		// It's so pretty...
		// I'm going to commit this before any more work.
		// Ahhh, it got even prettier:
		// g(n) = (5^(-n) (12^n (-218 + 49 log(3/2) + 60 log(144/25)) - 2 5^(n + 1) (7 n + 12) (log(12/5) - 1)))/(49 (log(3/2) - 2))
	};

	std::vector<long> extrapolated_ciura_tokuda =
	{
		1, 4, 10, 23, 57, 132, 305, 692, 1565, 3530, 7953, 17904, 40294, 90673, 204028, 459077, 1032939, 2324129, 5229307, 11765959, 26755942
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