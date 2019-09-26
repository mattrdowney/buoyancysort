#pragma once

#include <stack>
#include <stddef.h>
#include "natural-runs.h"

namespace Hierarchysort
{
	const long run_alignment = 8;
	const long run_alignment_mask = ~((1 << run_alignment) - 1);

	template <typename Type>
	long get_run(Type *data, long before_first, long after_last)
	{
		long end = NaturalRuns::direction_agnostic(data, before_first, after_last);
		long aligned_end = (before_first & run_alignment_mask) + 2 * run_alignment; // HACK: -1 & run_alignment_mask will result in undefined behavior later (e.g. 1s-complement vs 2s-complement)
		aligned_end = std::min(aligned_end, after_last);
		if (end < aligned_end)
		{
			InsertionSort::insert_block_from_right(data, before_first, aligned_end, end);
			end = aligned_end;
		}
		return end;
	}

	// I think merge follows this logic:
	// Take the run_size and add it to the contiguous run that is adjacent (all relevant cells).
	// Find the lower power of two (that's the largest size you'd want to create while preserving efficiency).
	// Bitshift 1 larger and subtract 1 to get a bitmask that will detect any collisions.
	// Use this against the VList of sorted cells (except the contiguous elements you already used) to detect a collision.
	// If there is a collision find the element using the XOR operator (i.e. ^) and power-of-two tricks (or most/least significant bits if an assembly instruction exists).

	// NOTE: merging a run often requires 2+ merges (O(lg(n)) on nearly-sorted (non-random) data.
	// I can call the MergeSort::merge() subroutine.
	// I will need to use extra data structures to store the state of the Hierarchy (which is probably my main source of dread).

	// Let's get information theoretical.
	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. While you could potentially do this in true O(nlg(n)) like Blocksort (note: the sizes of A and B are often not equal), for now I'm going to implement it with auxiliary memory // As an aside, it baffles me that Blocksort also uses the mergesort 1,2,4,8 merge system. I am pretty sure very few people know about this optimization based on the Mergesort page and other sources.
	{
		long size = after_last - (before_first + 1);
		long partially_sorted_size = 0;
		std::stack<long> run_stack; // using Timsort's naming convention (invariant 1: each bit size i.e. 1, 2, 4, 8, 16, 32... must only be used once (detect conflicts with XOR); invariant 2: bit sizes must be ordered (largest on the bottom of the stack, smallest at the top of the stack)
		// The more I think about it, the more I want to stick to powers of two.
		// 1) if there really is a lot of order, you probably won't e.g. for 127 and 1 have to do a ~64-position insertion sort on that single element
		// 2) it means hybrid (in-place and out-of-place) will work out-of-box without extra complications -- although you usually wouldn't switch from in-place to out-of-place often. Hmmm.
		long run_end = get_run(data, before_first, after_last);
		long run_size = first_run_end - (before_first + 1);
		if (unsorted_size > run_size)
		{
			run_stack.push(run_size);
			partially_sorted_size += run_size;

			while (partially_sorted_size < size)
			{
				long run_begin = run_end;
				run_end = get_run(data, run_begin - 1, after_last);
				run_size = run_end - run_begin;
				// TODO: This is the hard work
				partially_sorted_size += run_size;
			}
		}
	}
}


// Out-of-place hierarchysort is easier conceptually since it's just a question of how to merge lists in the following fashion:
// A: 1 2 4 8 16
// B: 1 2 4 8 16
// It's very similar to addition with carry bits.

// In-place hierarchysort on the other hand has uneven sizes:
// 1024 256 57 (16)
// In this case you would probably want to merge-in 7 elements to have a size 64 array. Basically, you only tolerate non-power-of-two sizes on the lowest level (the rest are powers of two).
// That being said, there's a lot of choices, and I think the best one should be similar to out-of-place hierarchysort.
// That probably means I will use an algorithm that's more similar to Mergesort than Timsort.
// This means that Hierarchysort still does merges of size 1 1 1 1 1 1 1 1 | 2 2 2 2 | 4 4 | 8, except it does them in a cache-coherent way with extra methods like OutlierSearch for quicker merging.

// Can this make performance markedly worse than Timsort? I don't think so, because Timsort's performance seems to be mostly bound by the number of inversions in the data.
// Thus Timsort will perform poorly if there are elements that are significantly displaced from their sorted order, because that means many of the early return conditions will mostly be unmet.

// It took me a while to figure this one out, but once I thought about it with respect to philosophy, e.g. "What version of in-place hierarchysort would be truest to the out-of-place version?", it started making more sense.
// Ironically, there's almost no differences between mergesort and hierarchysort, except that no one ever seemed to find the caching optimization (and OutlierSearch is a non-trivial discovery too, I suppose). Many of these optimizations either come from Timsort or might as well have.

// Hmmm, but if you think of Hierarchysort with natural runs there might be an interesting way to deal with natural runs and merging them.
// If you take the size 8 array example:
// ( 1 3 5 ) ( 2 6 8 ) ( 4 7 )
// Can be thought of as a Mergesort that has some extra rules to it.
// At the same time, I feel like this example would be better solved by mutating the array into (1 3)(2 5)(6 8)(4 7) and just doing a standard mergesort, so I'm probably overcomplicating it.

// I think the new in-place Hierarchysort would turn ( 1 3 5 ) ( 2 6 8 ) ( 4 7 ) => ( 1 2 3 5 ) ( 6 8 ) ( 4 7 ) => ( 1 2 3 5 ) ( 4 6 7 8 ) => ( 1 2 3 4 5 6 7 8)
// In the first step, (1 3 5) has a refinedness of 2/3 (across three positions) and one position has a refinedness of 1/3 for a total refinedness of (2/3)*3 + (1/3)*1 or (2+1/3)/4 which is greater than or equal to 50% refinedness so it needs to be merged.
// Obviously it would be cumbersome to step through all powers of 2^i at each merge position, so some extra logic would need to be made.

// It's worth mentioning this feels like a bad algorithm, because it always prefers to do insertion sorts for single elements.
// It does make the logic an run_stack simpler, though (at least I think it does) and natural runs don't matter too much after the lowest-level merges.
// Additionally, cache-coherency is relevant, and should be prioritized in most cases.


// Algorithm:
// get_run()
// get next power of two
// if the run is a power of two trivially merge it (when the bit-ordering allows it).
// if the run is not a power of two use recursion

// Eh, this is all trash.
// The main limitation Timsort dealt with was the O(lgn) GallopSearch on all iterations preventing it from doing well on nearly sorted data.
// Why not trust the data to sort itself?
// By this I mean, just use a Mergesort with blocks of 2^i (nothing fancy).
// When the adjacent information is nearly sorted then sort will require two comparisons to figure that out because of OutlierSearch().
// I do like the idea of getting natural runs from the data (which avoids some unnecessary comparisons), but I'll figure that one out as well.

// I think if I can find the equivalent of an accounting method ( https://en.wikipedia.org/wiki/Accounting_method_(computer_science) ) then I will be able to get this to work.

// On odd numbers (n & 1), do a merge of two size-1 arrays.
// On n mod 4 == 3 ((n & 3) == 3), do a merge of two size-2 arrays.
// And so on...
// You can skip this process if there is only a single array up to that point (since it is trivially merged).
// In practice, you have to do this continuously, e.g. for two size-6 arrays (1 2 5 6 8 10) (3 4 7 9 11 12) you cannot just check at the end of each run because there should be a merge of two size-4 arrays when the cursor hits position 7.

// How about:
// You can merge a run of size n with any other run of size (n/2, 2*n) [exclusive].
// Thus you just pull a run and find the smallest contiguous segment and try to merge with some fraction (maybe 100%) of it.
// You do this recursively until you run out of the current run or you run out of segments (in which case you greedily insert the current residue).

// Another thought, I should probably avoid runs that aren't multiples of run_alignment.
// It's a little extra work, but probably avoids a lot of small merges.

// If I'm correct (a rare occurence XD), Timsort on random data does not do power-of-two merges (but instead ~phi because of the invariants), which is why it generally does worse than Quicksort.
// Because of this, I think I should be concerned with making performance better on random data.
// If you think about it ( https://en.wikipedia.org/wiki/Timsort#Formal_verification ) the formal verification of Timsort talks about how you need to check the top four runs, and maybe I should do something similar.

// What would be elegant?
// I like falling back to the idea of merging in as many elements as possible and storing whether runs are contiguous.

// This means the data would look like:
// vector<int> vlist_position_occupied; // this is indexed by the bit position (i.e. little-endian) // stores zero when unoccupied. Otherwise it stores a unique index representing the contiguous chunk it is a part of.
// vector<long> vlist_contiguous_sizes; // this is indexed by the run position (i.e. big-endian)
// I think I can make this work, so I'll start coding with this as the basis of in-place hierarchysort.