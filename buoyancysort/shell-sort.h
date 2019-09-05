#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <stddef.h>
#include <vector>

namespace ShellSort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
		{
			long gap = gap_sequence[gap_index];
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

	template <typename Type>
	std::vector<long> gap_sequence_generator(std::function<long(Type)> lambda_function, long values)
	{
		std::vector<long> result;
		for (int value = 1; value <= values; value += 1)
		{
			result.push_back(lambda_function(value));
		}

		return result;
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

	std::vector<long> trying_to_beat_ciura1 =
	{
		1, 4, 10, 23, 57, 132, 311, 756, 1776 // I am happy with this sequence so far.
		// Minor changes may be needed but it follows the trend I want.
	};

	std::vector<long> trying_to_beat_ciura2 =
	{
		1, 4, 10, 23, 57, 132, 311, 750, 1786
	};

	std::vector<long> trying_to_beat_ciura3 =
	{
		// by tuples of 3: floor, ceil, floor, ceil, floor, ceil
		// starting from 1:
		//     n%3==1 implies you need a prime or 1,
		//     n%3==2 implies you need a number congruent to T(n)%(floor(n/3)+2)==0
		//     n%3==3 implies you need a number congruent to T(n)%(floor(n/3)+1)==0
		//     Try to prioritize large prime numbers in factorizations
		//1, 4, 10, 23, 57, 132, 313, 748, 1786,
		1, 4, 10, 23, 57, 132, 313, 724, 1772 // nope, didn't work
	};

	std::vector<long> trying_to_beat_ciura4 =
	{
		// by tuples of 3: floor, ceil, floor, ceil, floor, ceil
		// starting from 1:
		//     n%3==1 implies you need a prime or 1,
		//     n%3==2 implies you need a number congruent to T(n)%(floor(n/3)+2)==0
		//     n%3==3 implies do not modify
		1, 4, 10,
		23, 57, 132,
		313, 750, 1786,
		4259, 10201, 24139, //101 × 239
		57493, 136840, 326239, //311 × 1049
		777097
	};

	std::vector<long> trying_to_beat_tokuda1 =
	{
		// 1 3 8 20 47 111 264 630 1500 3572 8509 20269 48279 114999 273924 652478 1554181 3702006 8818053 21004303
		// 1 4 10 23 55 132 315 750 1786 4254 10134 24139 57499 136962 326239 777090 1851003 4409026 10502151 25015768
		1, 3, 8, 20, 49,
		118, 270, 637, 1509, 3583,
		8522, 20269, 48279, 114999, 273924,
		652478, 1554181, 3702006, 8818053, 21004303
	};

	std::vector<long> trying_to_beat_ciura5 =
	{
		//1 4 10 23 55 132 314 747 1779 4236 10086 24013 57169 136106 324035 771445 1836616 4372519 10409861 24783242
		1, 4, 10, 23, 57, 132, 314, 748, 1779, 4236, 10086, 24013, 57169, 136106, 324035, 771445
	};

	// TODO: variable primorial gap sequence using k#/i# (thus it e.g. sorts on 2, then 3, then 5 instead of 5 then 3 then 2)
	// primorial: 1, 2, 6, 30, 210, 2310, 30030, 510510,
	// "k#/i#": 510510/1, 510510/2, 510510/6, 510510/30, 510510/210, 510510/2310, 510510/30030, 510510/510510, (reverse ordered and only an example since it's variable)
	// upon thinking about it more, I feel like this wouldn't worth but I might as well try it if I have time.
	// Extending to other values that aren't arrays of size k#: 1) sort individual subarrays and merge or 2) double over with more primorials with remainder or 3) interpolate with standard gap sequences to get to primorial sorting

	// concept:
	// First 1 generalized_pratt<2> number (1)
	// Next 3 generalized_pratt<2, 3> numbers (4, 9, 36)
	// Next 5 generalized_pratt<3, 5, 7> numbers
	// Next 7 generalized_pratt<2, 3, 5, 7> numbers, etc
	// Next 9 generalized_pratt<2, 3, 5, 7, 11> numbers, etc
	std::vector<long> pratt_technique =
	{
		/*
		1,
		4, 9, 36,
		64, 125, 216, 512, 729,
		1296, 2401, 4096, 6561, 10000, 20736, 38416,
		59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000,
		531441, 1000000
		*/ // this was due to an error

		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304

		1,
		4, 9, 16,
		27, 64, 125, 216, 512,
		625, 1296, 2401, 4096, 6561, 10000, 20736,
		32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568
	};
	// If there's a reason to use an algorithm like this, it likely has to do with mitigating the worst case.
	// At the same time, I feel like Tokuda's algorithm also does well in the worst case.

	std::vector<long> hybridized_pratt_technique =
	{
		1, 4, 10, 23, 57, 132, 301, 701, // hybridization doesn't really affect the algorithm

		1296, 2401, 4096, 6561, 10000, 20736, 38416,
		59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000,
		531441, 1000000
	};

	std::vector<long> pratt_technique2 =
	{
		1,
		4, 9, 25, 36, // I was originally just adding that 25 then I realized I can use 1,2,3,4 instead of 1,3,5,7,9...
		64, 125, 216, 512, 729,
		1296, 2401, 4096, 6561, 10000, 20736, 38416,
		59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000,
		531441, 1000000
	};

	std::vector<long> pratt_technique3 =
	{
		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304
		1,
		4, 9,
		27, 64, 125,
		256, 625, 1296, 2401,
		3125, 7776, 16807, 32768, 59049,
		15625, 46656, 117649, 262144, 531441, 1000000
	};

	std::vector<long> pratt_technique4 =
	{
		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304
		1,
		4, 9,
		27, 64, 125, 216,
		256, 625, 1296, 2401, 4096, 6561, 10000, 20736,
		32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000
	};

	std::vector<long> pratt_technique5 = // hybridization
	{
		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304
		1, 4, 10, 23, 57, 132, 301, 701,

		1296, 2401, 4096, 6561, 10000, 20736,
		32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000
	};

	std::vector<long> pratt_technique6 =
	{
		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304
		1,
		4, 9, 16,
		27, 64, 125, 216, 512, 729, 1000,
		1296, 2401, 4096, 6561, 10000, 20736, 38416, 50625, 65536, 104976, 160000, 194481, 331776, 390625, 531441,
		537824, 759375, 1048576
	};

	std::vector<long> test = 
	{
		1, 4, 10, 29, 67
	};

	std::vector<long> pratt_technique7 =
	{
		//2-smooth: 1 2 4 8 16...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//5-smooth cubes: 1 8 27 64 125 216 512 729 1000 1728 3375 4096 5832 8000 13824 15625 19683 27000 32768 46656 64000 91125 110592
		//7-smooth quads: 1 16 81 256 625 1296 2401 4096 6561 10000 20736 38416 50625 65536 104976 160000 194481 331776 390625 531441 614656 810000 1048576 1500625 1679616
		//11-smooth pents: 1 32 243 1024 3125 7776 16807 32768 59049 100000 161051 248832 537824 759375 1048576 1889568 3200000 4084101 5153632 7962624 9765625 14348907 17210368 24300000 33554432
		//13-smooth hexas: 1 64 729 4096 15625 46656 117649 262144 531441 1000000 1771561 2985984 4826809 7529536 11390625 16777216 34012224 64000000 85766121 113379904 191102976 244140625 308915776 387420489 481890304

		// Wondering how to determine the crossover point for switching to the next sequence? Take the prime and raise it to the power of its sequence e.g. 2^2, 3^3, 5^4, 7^5, 11^6, 13^7

		1, 2, // 2
		4, 9, 16, // 3
		27, 64, 125, 216, 512, // 5
		625, 1296, 2401, 4096, 6561, 10000, // 6
		16807, 32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, // 9
		1771561, 2985984, 4826809, 7529536, 11390625, 16777216, 34012224, // 7
		62748517
	};

	std::vector<long> pratt_technique8 =
	{
		//2-smooth squares: 1 4 16 64 256...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//9^i*25^j: 1, 9, 25, 81, 225, 625, 729, 2025, 5625, 6561, 15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441,
		//25^i*49^j: 1, 25, 49, 625, 1225, 2401, 15625, 30625, 60025, 117649, 390625, 765625,
		//49^i*121^j: 1, 49, 121, 2401, 5929, 14641, 117649, 290521, 717409, 1771561, 5764801, 14235529, 35153041, 86806489,
		//121^i*169^j: 
		//169^i*289^j: 

		// Crossover point -- just take 1, 3, 5, 7, 9...

		1, // 1
		4, 9, 16, // 3
		25, 81, 225, 625, 729, // 5
		1225, 2401, 15625, 30625, 60025, 117649, 390625, // 7
		717409, 1771561, 5764801, 14235529, 35153041, 86806489
	};

	std::vector<long> pratt_technique9 =
	{
		//2-smooth squares: 1 4 16 64 256...
		//3-smooth squares: 1 4 9 16 36 64 81 144 256 324 576 729 1024 1296 2304 2916 4096 5184 6561 9216 11664 16384 20736 26244 36864
		//9^i*25^j: 1, 9, 25, 81, 225, 625, 729, 2025, 5625, 6561, 15625, 18225, 50625, 59049, 140625, 164025, 390625, 455625, 531441,
		//25^i*49^j: 1, 25, 49, 625, 1225, 2401, 15625, 30625, 60025, 117649, 390625, 765625,
		//49^i*121^j: 1, 49, 121, 2401, 5929, 14641, 117649, 290521, 717409, 1771561, 5764801, 14235529, 35153041, 86806489,
		//121^i*169^j: 
		//169^i*289^j: 

		// Crossover point -- just take 1, 2, 3, 4, 5...

		1, // 1
		4, 9, // 2
		25, 81, 225, // 3
		625, 1225, 2401, 15625, // 4
		117649, 290521, 717409, 1771561, 5764801, // 5
	};

	// Not doing anything with this right now, but:

	// I am going to seriously consider the "threading the needle" strategy again with a slight difference.
	// The (4-phi) value seems to be approximately right but something has always been off.
	// More recently I decided you want to alternate between multiplying (4-phi) +/- 1/e^(pi*(3/4))
	// What's the reason? I first tried alternating between 2.25 and 2.75 to try to get this behavior but it didn't converge correctly.
	// I did some trial and error until I remembered the 2.48 number from @invisal https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence
	// I checked ((4-phi)/sqrt(2.48))^2 which bounced around this mean pretty well by a distance of ~.1
	// (You can do this check by 2.48-(4-phi) and 2.28780728982-(4-phi))
	// I then checked what value was around ~.1 and found 1/e^(pi*(3/4)) is approximately around that value and it has served me well thus far
	// For the final check I verified that (((4-phi) + 1/e^(pi*(3/4)))^.5)*(((4-phi) - 1/e^(pi*(3/4)))^.5) is approximately (4-phi) -- it is approximately the same (I'll need to do more research to find the precise values if they exist).
	// Other things worth mentioning: it seemed like (4-phi) was correct but it also seemed like the answer should have been 99% of (4-phi) (another way of saying the value is wrong).
	// If you take the geometric average ((4-phi)+1/e^(3/4*pi))^(1/2) * ((4-phi)-1/e^(3/4*pi))^(1/2) you get a slight underestimate of this value (~2.38)
	// This difference is small, but the first value used is the smaller one and the +1 affects the sequence as well.
}