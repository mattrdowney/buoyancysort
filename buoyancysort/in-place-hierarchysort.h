#pragma once

#include <stddef.h>
#include "natural-runs.h"

namespace Hierarchysort
{
	const long run_alignment = 8;
	const long run_alignment_mask = ~((1 << run_alignment) - 1); // I feel like this concept inadvertently runs slower with random data (the original intention was to use [run_alignment, 2*run_alignment) runs -- and greater for natural runs).

	template <typename Type>
	long get_run(Type *data, long before_first, long after_last)
	{
		long end = NaturalRuns::direction_agnostic(data, before_first, after_last);
		long aligned_end = (before_first & run_alignment_mask) + 2 * run_alignment; // hilarious bug I noticed before running: -1 & run_alignment_mask is gonna be a crazy bug (or at least inconsistency) on certain systems (e.g. 1s-complement, could be wrong here). I think since I use before_first this lucks out and works as I want it to on random data.
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
	template <typename Type>
	void merge_run(Type *left, Type *right, long left_size, long right_size)
	{
		// NOTE: merging a run often requires 2+ merges (O(lg(n)) on nearly-sorted (non-random) data.
		// I can call the MergeSort::merge() subroutine.
		// I will need to use extra data structures to store the state of the Hierarchy (which is probably my main source of dread).

		/*
		long left_cursor = 0;
		long right_cursor = combined_input_size / 2;
		long output_cursor = 0;
		while (left_cursor < combined_input_size / 2 && right_cursor < combined_input_size)
		{
			if (run_input[left_cursor] <= run_input[right_cursor]) // ensures sort is stable
			{
				output[output_cursor] = run_input[left_cursor];
				left_cursor += 1;
			}
			else
			{
				output[output_cursor] = run_input[right_cursor];
				right_cursor += 1;
			}
			output_cursor += gap; // remember: the output_cursor also uses gaps.
		}

		// Deal with any remainder (no need to compare anymore)
		while (left_cursor < combined_input_size / 2)
		{
			output[output_cursor] = run_input[left_cursor];
			left_cursor += 1;
			output_cursor += gap;
		}
		while (right_cursor < combined_input_size)
		{
			output[output_cursor] = run_input[right_cursor];
			right_cursor += 1;
			output_cursor += gap;
		}
		*/
	}

	// Let's get information theoretical.
	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. While you could potentially do this in true O(nlg(n)) like Blocksort (note: the sizes of A and B are often not equal), for now I'm going to implement it with auxiliary memory // As an aside, it baffles me that Blocksort also uses the mergesort 1,2,4,8 merge system. I am pretty sure very few people know about this optimization based on the Mergesort page and other sources.
	{
		/*
		long size = after_last - (before_first + 1);
		// Data structure: VList (conceptually, not actually) storing power-of-two pairs of interlaced lists i.e. (a,b) represented as (a0, b0, a1, b1, a2, b2, a3, b3...)
		// The data layout might be inefficient-looking, but it does help with cache prefetching, which is why I include it.
		long vlist_size = 2 * PowerOfTwo::upper_power_of_two(size); // TODO: this might overshoot on powers of two (I don't think so) so check it.
																	// *2 because the geometric sequence 1 + 2 + 4 + ... n/4 + n/2 + n ~= 2n
																	// *2 because you need two lists total
																	// next power of two because you need enough space to store n.
																	// As an aside, I really feel like this should be 2 not 4, because the geometric series should only go up to n/2.
		std::vector<Type> vlist(vlist_size); // A VList is a data structure (this is technically not a VList, it's closer to a re-implementation I made); this has interlaced an contiguous properties.
		long vlist_elements = 0;

		// Let's say you want to merge 16 (0b'0001'0000) elements into 240 (0b'1111'0000) elements. You bitwise AND (i.e. & operator) (0b'0001'0000) with (0b'1111'0000). Because there is a conflict, you place the elements into slot b and merge (conceptually, although you can skip the redundant copy). You can then bitshift up and notice a conflict again, etc, so merge. At the end add 16 and 240 to get 256 elements.
		while (vlist_elements + double_run_size < size) // < instead of <= is intentional for a minor optimization for powers of two (it saves an O(n) copy operation which is significant).
		{
			// Sort two runs to be combined and inserted into the VList.
			InsertionSort::sort(data, before_first + vlist_elements, (before_first + 1) + vlist_elements + run_size);
			InsertionSort::sort(data, before_first + vlist_elements + run_size, (before_first + 1) + vlist_elements + double_run_size);

			long merge_counter = double_run_size;
			// You can do a lot of cool stuff with bit shifting. To get a VList index from a size you basically just bitshift/multiply by 2 (2 because there are 2n elements in the VList). If you need the second list just add 1 since they are interlaced.
			bool second = merge_counter & vlist_elements;
			Type *input;
			Type *output = vlist.data() + double_run_size * 2 + (second ? 1 : 0);
			// You can now merge the two runs into the first or second slot of the VList at the "smallest" position.
			run_merge(output, &data[vlist_elements], double_run_size);

			input = &output[-1];
			while (second)
			{
				merge_counter *= 2;
				second = merge_counter & vlist_elements;
				Type *output = vlist.data() + 2 * merge_counter + (second ? 1 : 0);
				interlaced_merge(output, input, merge_counter);
				input = &output[-1]; // because the output was aligned one to the right because of "second"
			}
			vlist_elements += double_run_size;
		}
		InsertionSort::sort(data, before_first + vlist_elements, after_last);
		if (vlist_elements > double_run_size) // <= double_run_size just ends up being an insertion sort.
		{
			// find all the chunk sizes that are occupied as a vector.
			// E.g. 240 would be (16, 32, 64, 128)
			long merged_size = size - vlist_elements;
			long remainder = double_run_size;
			while (vlist_elements > 0)
			{
				if (remainder & vlist_elements) // check if the VList contains that particular list size
				{
					merge_remainder(&data[after_last - merged_size - remainder], &vlist[2 * remainder], merged_size, remainder);
					vlist_elements -= remainder;
					merged_size += remainder;
				}
				remainder *= 2;
			}
		}
		*/
	}
}