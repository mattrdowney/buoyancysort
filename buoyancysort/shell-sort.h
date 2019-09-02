#pragma once

#include <algorithm>
#include <functional>
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

	std::vector<long> primorial_sort = // a slower growing function instead of the primorial: n! where some_prime = p(n)
	{
		1, /*2,*/ 4, // from 2-smooth #s
		9, 16, /*connecting seam post-30*/ 36, // from 3-smooth squares
		64, 125, /*connecting seam post-210*/ 216, // from 5-smooth cubes
		256, 625, 1296, 2401, 4096, 6561, 10000, 20736, // from 7-smooth quads
		32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000 // from 11-smooth quints
	}; // not bad at all without hybridization

	// Hybridization doesn't offer much of an improvement
	std::vector<long> hybridized_primorial_sort = // a slower growing function instead of the primorial: n! where some_prime = p(n)
	{
		1, 4, 10, 23, 57, 132, 301, 701,
		1296, 2401, 4096, 6561, 10000, 20736, // from 7-smooth quads
		32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576, 1889568, 3200000 // from 11-smooth quints
	};

	// Hmmm, but there's an interesting idea...
	// Instead of doing this I can do the following:
	// prefix with 1
	// Next 3 generalized_pratt<2,3> numbers (4, 9, 36)
	// Next 5 generalized_pratt<3,5> numbers
	// Next 7 generalized_pratt<5,7> numbers, etc

	// I'd want to make sure the primes grow about as quickly as the "3, 5, 7" sequence (whatever the final version looks like)

	// I think I'll try the following next:
	// UNION(primorial, compositorial)
	// (in practice this function would also need a function that does the following -- add a e.g. Tokuda gap sequence until the max gap number - 2 (instead of using the two largest gap numbers).
	// Another idea I got thanks to a cool hobbiest here: https://stackoverflow.com/questions/21508595/shellsort-2-48k-1-vs-tokudas-sequence
	// I think they might have found something like the equivalent for Tokuda: ceil( [(43^k) - (7^k)] / [(43 - 7) * (7^(k-1)) ] )
	// How did I determine this? I took 2.48^2 and got a ~6.15 number (which would be the ratio of the two numbers).
	// I then instinctively thought 7*.15 ~= 1, the result was a prime (43), so it passed the gut check.
	// Now to test: (thanks @invisal from stackoverflow)
	// (If this works, I wonder if it has anything to do with the Lucky numbers of Euler, specifically: 5+2, 41+2)
	// (If that's the case the "best" sequence could be 43^2/3^3 which is an absurd 205.444444 ratio. *shrugs*)
	// Without further ado.

	std::vector<long> invisal_gap_sequence1 =
	{
		1, 39, 1463, 55194 // grows really quickly unfortunately (because I did it wrong XD)
	};

	// oh wait I went in the wrong direction, I did squared instead of a square root.
	// Thus you get 7 and 11 (assuming I had the right idea)

	std::vector<long> invisal_gap_sequence2 = // for real this time, ceil( ((b^k) - (a^k)) / ((b - a) * (a^(k-1)) ) ), a=7^2, b=11^2, k=1
	{
		1, 4, 10, 25, 62, 154, 381, 941, 2323, 5738, 14169, 34989, 86402, 213360, 526869, 1301045
	};
	// Lol, if this invisal person found a better Ciura sequence >5.5 years ago I'm gonna be ecstatic.
	// I don't know why people don't take internet hobbiests seriously (I think they are amazing at least).
	// Testing...
	// Awww, it does slightly worse (I don't know if my generating formula is right, though, maybe it's not b-a ... wait I meant b^2 - a^2 (although maybe the formula is b+a)
	// Why am I so bad at this lol

	std::vector<long> invisal_gap_sequence3 = // even more real this time, ceil( ((b^k) - (a^k)) / ((b + a) * (a^(k-1)) ) ), a=7^2, b=11^2, k=1
	{
		1, 2, 5, 11, 27, 66, 162, 399, 984, 2430, 6001, 14819, 36594, 90365, 223145, 551031
	};

	std::vector<long> invisal_gap_sequence4 = // as real as it can get?, ceil( ((b^k) - (a^k)) / ((b^2 - a^2) * (a^(k-1)) ) ), a=7^2, b=11^2, k=1
	{
		1, 1, 1, // yeah that didn't work
	};

	// Oh wait, I suck at life (Wolfram Alpha has made me lazy).

	// A-ha so it was 11 and 17 for the Ciura sequence (I really wouldn't have found this without invisal so I'd like to attribute this to their stackoverflow post.
	std::vector<long> invisal_gap_sequence6 = // mwahahaha - ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=11^2, b=17^2, k=6
	{
		1, 4, 10, 23, 56, 133, 319, 763, 1822, 4351, 10392, 24821, 59284, 141596, 338192, 807747, //it may look like Ciura's sequence but the differences hurt it.
	};

	// screw it, trying 17, 41
	std::vector<long> invisal_gap_sequence7 = // ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=17^2, b=41^2, k=1
	{
		1, 7, 41, 238, 1383, 8041, 46768, 272031, 1582295 // markedly worse performance.
	};

	// Maybe I'm missing something fundamental that would give me the sequence with invisal_gap_sequence6 (a lopsidedness that generally overestimates)
	// At the very least, I know the ratio is near 17^2/11^2 for Ciura's sequence (2.38842975...)
	// Now to brute-force test ratios and see if any are similar
	// (I say brute force, but you can be a little smart about it; start at 2 and go through the primes and check a position that's about that distance away)
	// 19/29 work well (at least they underestimate)
	
	std::vector<long> invisal_gap_sequence8 = // ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=19^2, b=29^2, k=1
	{
		1, 4, 9, 22, 51, 120, 280, 652, 1520, 3541, 8250, 19219, 44773, 104306, 242995, 566091
	};

	// Personal oppinion via looking at ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=1^2, b=(1+sqrt(2))^1, k=7 for a couple of minutes
	// The number we're looking for is the silver ratio.
	// Yes, it overestimates Ciura, but it seems like it's on the mark. There's no guarantee I'm using the right formula either (although I have a ton of respect for Tokuda's equation so I wouldn't be surprised).
	
	std::vector<long> invisal_gap_sequence9 = // ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=1^2, b=(1+sqrt(2))^1, k=3
	{
		1, 4, 10, 24, 58, 140, 338, 816, 1970, 4756, 11482, 27720, 66922, 161564, 390050, 941664
	};

	// I am wrong, as usual

	// I'm pretty confident about this one too: ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=1^2, b=(4-1.618033)^1, k=9

	std::vector<long> invisal_gap_sequence10 = // ceil( ((b^k) - (a^k)) / ((b-a) * (a^(k-1)) ) ), a=1^2, b=(4-1.618033)^1, k=9
	{
		// Once I saw this, it felt like it should be on oeis.org but it isn't
		1, 4, 10, 23, 55, 132, 315, 750, 1786, 4254, 10134, 24139, 57500, 136963, 326241, 777095 // as an aside, my gut said this sequence was it (probably because it felt like some weird combination of Fibonacci numbers and n^2 + 4
	}; // I guess it isn't it. Now that I think about it 750 is a lot bigger than 701

	// I guess I should have been looking for 2.356 all along. I'm trying to think what is around that value.
	// Playing around with the number I finally got pi*(3/4) as a pretty good approximation, so let's see if it works
	// ceil(((b^k) - (a^k)) / ((b - a) * (a ^ (k - 1)))), a = 1 ^ 2, b = ((pi*(3 / 4))) ^ 1, k = 1

	std::vector<long> invisal_gap_sequence11 = // ceil(((b^k) - (a^k)) / ((b - a) * (a ^ (k - 1)))), a = 1 ^ 2, b = ((pi*(3 / 4))) ^ 1, k = 1
	{
		// it really excites me that pi/tau is used in this sequence 
		1, 4, 9, 22, 53, 126, 297, 700, 1650, 3888, 9162, 21588, 50865, 119849, 282387, 665359
	};

	std::vector<long> invisal_gap_sequence12 =
	{
		// floor ceil floor ceil floor ceil
		1, 5, 7, 23, 53, 131,
		313, 751, 1783, 4253, 10133, 24151,
		57493, 136963, 326219, 777097, 1850987, 4409033
		// From equation:
		// double unrounded = (pow(b, n) - 1) / (b - 1);
		// long rounded = (n % 2) == 0 ? floor(unrounded) : ceil(unrounded);
		// return rounded;
		// Then, you just need to find the floor'ed or ceil'ed next/previous prime.
	};

	std::vector<long> invisal_gap_sequence13 =
	{
		// floor ceil floor ceil floor ceil
		1, 4, 10, 23, 53, 131,
		313, 751, 1783, 4253, 10133, 24151,
		57493, 136963, 326219, 777097, 1850987, 4409033
	};

	std::vector<long> invisal_gap_sequence14 = // v = (4 - 1.6180339887498948482)*n + (1/1.6180339887498948482), n=273923.79944296375930
	{
		1, 3, 8, 20, 47, 111, 264, 630, 1500, 3572,
		8509, 20269, 48279, 114999, 273924, 652478
	};

	std::vector<long> invisal_gap_sequence15 = // v = (4 - 1.6180339887498948482)*n + 1.6180339887498948482, n=1
	{
		1, 4, 12, 29, 69, 166, 396, 944, 2249, 5358, 12763, 30402, 72418, 172498, 410886, 978717
	};

	std::vector<long> trying_to_beat_ciura =
	{
		
		// Original sequence: 1, 4, 10, 23, 55, 132, 316, 754, 1797
		// From: value = b * value + pow(1 + 1.0/535.491656, n-1), b = 4 - phi
		// 55 and 132 share a common factor (11) so change one of them
		// 1, 4, 10, 23, 55, 131, 316, 754, 1797
		// All the other prime factorizations are good (4/10 and 316/754 share a factor of 2)
		// Attempt didn't work: 1, 4, 10, 23, 55, 131, 316, 754, 1797
		// Try to match Ciura and deconflict 55/132 like Ciura's sequence
		// 1, 4, 10, 23, 53, 132, 316, 754, 1797 -- that did significantly worse
		// Try to deconflict 316/754 even though they only share one common factor
		// 1, 4, 10, 23, 55, 131, 316, 751, 1797 -- going to a prime does significantly worse
		// 1, 4, 10, 23, 55, 131, 316, 753, 1797 // -- going down 1 does significantly better
		// 1, 4, 9, 23, 55, 131, 316, 753, 1797 -- just in case, checking if deconflicting the 10 does better (it doesn't)
		1, 4, 10, 23, 55, 131, 316, 753, 1797
	};

	//I really should've made this function a long time ago...
	
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

	//I got a better idea on how to find this: a=1^2, b=2.3^2 (I think I just need the ratio)

	// My intuition says the best path forward is to work on the 4^i*9^j version. (Someone else can always test other possibilities; I would want to write code to test them via brute force if I actuallly went down that path)
}