#pragma once

#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <vector>
#include "natural-runs.h"

namespace Hierarchysort
{
	const long run_bit_alignment = 4; // Starts from 0 (0 implies no alignment)
	const long run_alignment = (1 << run_bits);

	template <typename Type>
	long get_run(Type *data, long before_first, long after_last)
	{
		long end = NaturalRuns::direction_agnostic(data, before_first, after_last);
		long aligned_end = (before_first + 1) + run_alignment;
		if (end < aligned_end && end < after_last)
		{
			InsertionSort::insert_block_from_right(data, before_first, aligned_end, end);
			end = aligned_end;
		}
		return end;
	}

	// This is what I called hard O(n) adaptive best-case performance -- caveat I think I'm missing some optimizations that would make the adaptive performance better. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. While you could potentially do this in true O(nlg(n)) like Blocksort (note: the sizes of A and B are often not equal), for now I'm going to implement it with auxiliary memory // As an aside, it baffles me that Blocksort also uses the mergesort 1,2,4,8 merge system. I am pretty sure very few people know about this optimization based on the Mergesort page and other sources.
	{
		long size = after_last - (before_first + 1);
		long bit_size = std::ceil(std::log(size)) + 1 /*because of floating-point imprecision, I'm adding some room for error*/ + 1;
		std::vector<int> vlist_position_occupied(bit_size); // this is indexed by the bit position (i.e. little-endian) // stores zero when unoccupied. Otherwise it stores a unique index representing the contiguous chunk it is a part of.
		std::vector<Type> auxiliary_buffer(size); // If this weren't initialized to size, you couldn't memcpy safely.

		for (long bit = 0; bit < bit_size; bit += 1)
		{
			vlist_position_occupied[bit] = INT_MAX; // This is more convenient than zero.
		}

		long unique_segments = 0;
		long partially_sorted_size = 0;
		long run_end = 0;
		while (partially_sorted_size < size)
		{
			long run_begin = run_end;
			long run_end = get_run(data, run_begin - 1, after_last);
			long run_size = run_end - run_begin;
			partially_sorted_size += run_size; // NOTE: I could do this after the merge but it's more convenient if I can mutate run_size
			while (run_size > 0)
			{
				long bit = run_bit_alignment;
				bool trivial_merge = true;
				long left_size = 0;
				long right_size = run_alignment;
				while (true)
				{
					if (vlist_position_occupied[bit] < unique_segments) // bit belongs to a second contiguous set (you cannot safely merge beyond this point)
					{
						// "bit" position will always be one-past the end here
						break;
					}
					if (vlist_position_occupied[bit] == unique_segments)
					{
						vlist_position_occupied[bit] = INT_MAX; // merge in the values (unset the bit (to the sentinel value)) to show the position is unoccupied.
						left_size += (1 << bit);
						bit += 1;
						trivial_merge = false;
						continue;
					}
					long extrapolated_size = right_size + (1 << bit);
					if (extrapolated_size <= run_size)
					{
						// bit must be unused at this point (because the value is INT_MAX, the sentinel value representing unused vlist positions).
						right_size = extrapolated_size;
						//if (extrapolated_size < run_size)
						//{
							bit += 1;
						//}
					}
					if (extrapolated_size >= run_size)
					{
						// "bit" position will always be one-past the end here // TODO: VERIFY: I don't know if this is right...
						break;
					}
				}
				if (trivial_merge)
				{
					if (unique_segments == 0 || data[run_begin - 1] >= data[run_begin])
					{
						// You don't actually have to search for the segment, if you know it exists, you just get the element before run_begin. (If it doesn't exist you can get a segmentation fault.)
						// If the data is not trivially sorted, then you have a new segment.
						unique_segments += 1;
					}

					// Set all of the bits that are being merged in.
					for (; bit >= run_alignment_bits; bit -= 1)
					{
						long subarray_size = (1 << bit);
						if (subarray_size & run_size)
						{
							vlist_position_occupied[bit] = unique_segments;
						}
					}
				}
				else // standard merge (possibly cascading)
				{
					long merge_begin = run_begin;
					long merge_left = left;
					long merge_right = right;
					long merge_bit = bit;
					while (true)
					{
						// A unique segment might have been deleted, so find the number of unique_segments
						unique_segments = 0; // This can happen (e.g. during a merge of two size 16 arrays). Otherwise the value will be set in the for loop.
						for (long segment = bit; bit < bit_size; segment += 1)
						{
							if (vlist_position_occupied[bit] != INT_MAX)
							{
								unique_segments = vlist_position_occupied[bit]; // You could do this concept using indices/"pointers" and it would probably be more elegant.
								break;
							}
						}
						// TODO: OutlierSearch optimization
						std::memcpy(auxiliary_buffer.data(), &data[merge_begin], merge_right * sizeof(Type));
						MergeSort::leftward_merge(&data[merge_begin - merge_left], &data[merge_begin], auxiliary_buffer.data(), merge_left + merge_right, merge_left, merge_right);
						merge_begin -= merge_left;
						merge_right += merge_left;
						merge_left = 0;
						if (unique_segments > 0)
						{
							for (long segment = bit; bit < bit_size; segment += 1)
							{
								if (vlist_position_occupied[bit] != unique_segments)
								{
									break;
								}
								merge_left += (1 << segment);
								merge_bit += 1;
							}
						}
						if (merge_left <= 0)
						{
							if (unique_segments == 0 || data[run_begin - 1] >= data[run_begin])
							{
								unique_segments += 1;
							}
							vlist_position_occupied[merge_bit] = unique_segments;
							break;
						}
					}
				}

				run_begin += right_size;
				run_size -= right_size;
			}
		}
	}
}