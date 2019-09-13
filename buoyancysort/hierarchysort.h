#pragma once

#include <stddef.h>
#include <string.h>
#include "power-of-two.h"
#include "print.h"

namespace Hierarchysort
{
	const long run_size = 8; // this doesn't actually have to be a power of two, but I think it's easier to teach/implement when it is. // As an aside, I wonder if the reason Insertion Sort tends to work well with size 4-16 (expected 8) arrays has to do with the lg(e) term in Stirling's approximation, which is related to the coincidental order in random permutations (the term might be 2^(lg(e^2)) or ~7.39) -- although I'm at least a little wrong because complexity coefficients are relevant too.
	const long gap = 2;
	const long double_run_size = run_size * gap;

	// essentially this is vector-to-vlist
	template <typename Type>
	void run_merge(Type *output, Type *run_input, long combined_input_size)
	{
		long left_cursor = 0;
		long right_cursor = combined_input_size/2;
		long output_cursor = 0;
		while (left_cursor < combined_input_size/2 && right_cursor < combined_input_size)
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
		while (left_cursor < combined_input_size/2)
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
	}

	// essentially this is vlist-to-vlist
	template <typename Type>
	void interlaced_merge(Type *output, Type *interlaced_input, long combined_input_size)
	{
		long left_cursor = 0;
		long right_cursor = 1;
		long output_cursor = 0;
		while (left_cursor < combined_input_size && right_cursor < combined_input_size)
		{
			if (interlaced_input[left_cursor] <= interlaced_input[right_cursor]) // ensures sort is stable
			{
				output[output_cursor] = interlaced_input[left_cursor];
				left_cursor += gap;
			}
			else
			{
				output[output_cursor] = interlaced_input[right_cursor];
				right_cursor += gap;
			}
			output_cursor += gap; // remember: the output_cursor also uses gaps.
		}

		// Deal with any remainder (no need to compare anymore)
		while (left_cursor < combined_input_size)
		{
			output[output_cursor] = interlaced_input[left_cursor];
			left_cursor += gap;
			output_cursor += gap;
		}
		while (right_cursor < combined_input_size)
		{
			output[output_cursor] = interlaced_input[right_cursor];
			right_cursor += gap;
			output_cursor += gap;
		}
	}

	// essentially this is vlist-to-vector
	// Because the way the VLists are populated, we can still have stability if we do merge operations in reverse-order (which is the most efficient anyway).
	template <typename Type>
	void merge_remainder(Type *contiguous_input_output, Type *interlaced_input, long contiguous_size, long interlaced_size)
	{
		long left_cursor = 0; // the left cursor is the interlaced_input (it was populated first which is why it's on the left). (It's also most convenient to put it on the left, if you prefer thinking about it that way.)
		long right_cursor = interlaced_size; // The right cursor is from the real information in the contiguous_input_output -- half-ish is gibberish // (it's hard to grok, to an extent, why things work).// it's starting at interlaced_size because that's the amount of giberish data and the contiguous_input_output is supposed to store data of size contiguous_size + interlaced_size (only half of which is meaningful information at the beginning of the function call).
		long output_cursor = 0;
		while (left_cursor <  2*interlaced_size && right_cursor < interlaced_size + contiguous_size)
		{
			// With respect to keeping half the data, this is an optimization present in Timsort.
			// Copy one of the two arrays (in this case it was implicitly done by the interlaced VLists beforehand)
			// into an auxiliary buffer (usually the smaller but here we use the larger)
			// to allow an easy "in-place" (*not actually in-place*) merge.
			if (interlaced_input[left_cursor] <= contiguous_input_output[right_cursor]) // ensures sort is stable
			{
				contiguous_input_output[output_cursor] = interlaced_input[left_cursor];
				left_cursor += gap; // only the left_cursor is interlaced (the others use contiguous data).
			}
			else
			{
				contiguous_input_output[output_cursor] = contiguous_input_output[right_cursor];
				right_cursor += 1;
			}
			output_cursor += 1;
		}

		// Deal with any remainder (no need to compare anymore)
		while (left_cursor < 2*interlaced_size)
		{
			contiguous_input_output[output_cursor] = interlaced_input[left_cursor];
			left_cursor += gap;
			output_cursor += 1;
		}
		// because of the nature of doing everything "in-place" you don't actually need to copy the righthand side (it's implicitly there)
		//while (right_cursor < contiguous_size)
		//{
		//	contiguous_input_output[output_cursor] = contiguous_input_output[right_cursor];
		//	right_cursor += 1;
		//	output_cursor += 1;
		//}
	}

	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. What I think of as in-place isn't actually in-place if I remember correctly (once I realized in-place merge costs klg(k)).
	{
		// This one has by far the coolest optimizations -in my opinion- (although I think there's a bit more overhead related to them, so you probably want to e.g. insertion sort 32 element chunks instead of 16).
	}

	// This is what I called soft O(n) adaptive best-case performance. This is better with random data (I have to test this theory, though).
	template <typename Type>
	void out_of_place(Type *data, long before_first, long after_last) // conceptually, this is little-endian
	{
		long size = after_last - (before_first + 1);
		// Data structure: VList (conceptually, not actually) storing power-of-two pairs of interlaced lists i.e. (a,b) represented as (a0, b0, a1, b1, a2, b2, a3, b3...)
		// The data layout might be inefficient-looking, but it does help with cache prefetching, which is why I include it.
		long vlist_size = 2*2*PowerOfTwo::upper_power_of_two(size); // TODO: this might overshoot on powers of two (I don't think so) so check it.
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


			Print::print(data, -1, after_last);
			Print::print(vlist.data(), -1, vlist.size());

			long merge_counter = double_run_size;
			// You can do a lot of cool stuff with bit shifting. To get a VList index from a size you basically just bitshift/multiply by 2 (2 because there are 2n elements in the VList). If you need the second list just add 1 since they are interlaced.
			bool second = merge_counter & vlist_elements;
			Type *input;
			Type *output = vlist.data() + double_run_size * 2 + (second ? 1 : 0);
			// You can now merge the two runs into the first or second slot of the VList at the "smallest" position.
			run_merge(output, &data[vlist_elements], double_run_size);

			Print::print(data, -1, after_last);
			Print::print(vlist.data(), -1, vlist.size());
			input = &output[-1];
			while (second)
			{
				merge_counter *= 2;
				second = merge_counter & vlist_elements;
				Type *output = vlist.data() + 2*merge_counter + (second ? 1 : 0);
				interlaced_merge(output, input, merge_counter);
				input = &output[-1]; // because the output was aligned one to the right because of "second"

				Print::print(data, -1, after_last);
				Print::print(vlist.data(), -1, vlist.size());
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
					merge_remainder(&data[after_last - merged_size - remainder], &vlist[2*remainder], merged_size, remainder);
					vlist_elements -= remainder;
					merged_size += remainder;
				}
				remainder *= 2;

				Print::print(data, -1, after_last);
				Print::print(vlist.data(), -1, vlist.size());
			}
		}
	}

	// As a hypothetical, this could be similar to an out-of-place hierarchysort concept I was considering (based on what I remember there was some interesting zig-zagging). One idea I had was to overlay power-of-two lists (a,b) like so (a0, bn, a1, bn-1, a2, bn-2, a3, bn-3 ... an-3, b3, an-2, b2, an-1, b1, an, b0). That being said, I remember I was working on multiple concepts when I started on this, so I shouldn't lock anything into place yet since I'm essentially in the discovery stage.
	template <typename Type>
	void hybrid(Type *data, long before_first, long after_last) // conceptually this is a hybrid of big-endian and little-endian (supposed to be adaptive in both the real and programmer sense of the word). (This was a work-in-progress.)
	{
	}
}