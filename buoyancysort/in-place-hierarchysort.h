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