#pragma once

#include <math.h>
#include <stddef.h>
#include <vector>
#include "natural-runs.h"

namespace Hierarchysort
{
	const long run_bits = 4;
	const long run_alignment = (1 << run_bits);

	template <typename Type>
	long get_run(Type *data, long before_first, long after_last, long &residue_index) // TODO: this is broken (terribly)
	{
		long residue_cursor = (residue & run_alignment_mask) + 2 * run_alignment;
		if (residue_cursor - before_first >= run_alignment)
		{
			return residue_cursor;
		}
		long end = NaturalRuns::direction_agnostic(data, residue_index /*this is before_first*/, after_last);
		// TODO: merge in the residue with the next run (up to run_alignment) then check if the two can be trivially merged.
		long aligned_end = (before_first & run_alignment_mask) + 2 * run_alignment; // HACK: -1 & run_alignment_mask will result in undefined behavior later (e.g. 1s-complement vs 2s-complement)
		aligned_end = std::min(aligned_end, after_last);
		if (end < aligned_end)
		{
			InsertionSort::insert_block_from_right(data, before_first, aligned_end, end);
			end = aligned_end;
		}
		return end;
	}

	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	// While this is certainly complicated, it is probably a third as difficult as I thought it would be.
	// The code seems pretty elegant (you don't have to think about 50 moving parts at once).
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. While you could potentially do this in true O(nlg(n)) like Blocksort (note: the sizes of A and B are often not equal), for now I'm going to implement it with auxiliary memory // As an aside, it baffles me that Blocksort also uses the mergesort 1,2,4,8 merge system. I am pretty sure very few people know about this optimization based on the Mergesort page and other sources.
	{
		long size = after_last - (before_first + 1);
		long bit_size = std::ceil(std::log(size)) + 1 /*because of floating-point imprecision, I'm adding some room for error*/ + 1;
		std::vector<int> vlist_position_occupied(bit_size); // this is indexed by the bit position (i.e. little-endian) // stores zero when unoccupied. Otherwise it stores a unique index representing the contiguous chunk it is a part of.

		int vlist_indices = 0;
		long residue_index = -1;
		long partially_sorted_size = 0;
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
				partially_sorted_size += run_size; // I could do this after the merge to be more formal, but whatever (run_size is modified, so it's easier to change here)
				if (run_size <= run_alignment)
				{
					// "simple" merge in (which isn't that simple since you still need to properly use the data structures and take into account all of the other optimizations (e.g. OutlierSearch, contiguous regions, gallop merge (last one is a stretch goal))
				}
				else
				{
					while (run_size > 0)
					{
						bit = run_bits; // 16 (1 << 4) is actually the 5th position since we start from 0.
						trivial_merge = true;
						vlist_size = 0;
						mix_in_size = run_alignment;
						while (true)
						{
							if (bit belongs to a second contiguous set)
							{
								break;
							}
							if (bit is used by vlist)
							{
								vlist_size += (1 << bit);
								bit += 1;
								trivial_merge = false;
								continue;
							}
							test_size = mix_in_size + (1 << bit);
							if (test_size <= run_size)
							{
								// bit must be unused at this point
								mix_in_size = test_size;
								bit += 1;
							}
							if (test_size >= run_size)
							{
								break;
							}
						}
						if (!trivial_merge)
						{
							// Note, the merge is actually more complicated:
							// 1) it cascades
							// 2) you need to make sure cascading chunks take contiguous optimization into account
							// 3) if you want to be cache-friendly you have to do the memory zig-zag
							// 4) even if you don't want to be cache-friendly, you are doing this in-place so MergeSort::merge() doesn't work out-of-box, you need an auxiliary buffer.
							MergeSort::merge(-vlist_size - , -mix_in_size - );
						}
						//     set and unset bits in vlist_position_occupied as necessary (you can actually perform this work throughout the while (true) loop, you just want to be careful (and I won't be while making pseudocode).
						run_size -= mix_in_size;
					}
				}
			}
		}
	}
}