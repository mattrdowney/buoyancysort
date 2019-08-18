#pragma once

#include <algorithm>
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
			for (long unsorted_cursor = (before_first+1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
			{
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

	std::vector<long> ciura_gap_sequence = { 1750, 701, 301, 132, 57, 23, 10, 4, 1 }; // NOTE: 1750 is an (un-official?) extension
	// I think I was wrong about the square_root(5) part, I think the number I'm looking for might be slightly larger.

	std::vector<long> root_five_prime_gap_sequence = { 631, 281, 127, 59, 29, 13, 5, 3, 1 };

	std::vector<long> root_five_silver_ratio_gap_sequence1 = { 1448, 629, 293, 125, 68, 29, 13, 5, 1 }; // this does ceil without epsilon // NOTE: misuse of the word "silver ratio"
	std::vector<long> root_five_silver_ratio_gap_sequence2 = { 1448, 629, 293, 148, 68, 29, 13, 5, 1 }; // this does ceil with epsilon // closest to competing with ciura_gap_sequence (but still seems worse)

	std::vector<long> root_five_gap_sequence1 = { 624, 278, 124, 54, 24, 10, 4, 1 };
	std::vector<long> root_five_gap_sequence2 = { 625, 279, 125, 55, 25, 11, 5, 1 };
	std::vector<long> root_five_gap_sequence3 = { 625, 280, 125, 56, 12, 5, 3, 1 };

	std::vector<long> two_and_a_quarter_silver_ratio_gap_sequence = { 1525, 680, 293, 148, 68, 29, 13, 5, 1 };
	// I like this sequence; it seems to beat ciura_gap_sequence when you set the max gap sequence values (i.e. the largest) to be identical (which I consider a fair-er fight)
	// The 2.25 value might still be an underestimate, or maybe the ratio isn't static over time (I'm not going to guess which theory is correct)
	// E.g. if you take the Wikipedia example (different random order of https://en.wikipedia.org/wiki/Shellsort#/media/File:Sorting_shellsort_anim.gif 's size-42 array) with or without a fair-er fight (i.e. replacing 29 with 23) my version slightly out-performs either way.
	// I would need to do more testing to know, and this (generally) isn't worth thinking about, but I can't help being curious here.
	// I feel like this principle would be a little better if it were dynamic while preserving the rest of the behavior
	// (e.g. starting with ~N/sqrt([ratio]) and finding the next largest silver ratio, then decreasing by a factor of [ratio] and finding the next silver ratio on each iteration thereafter -- just be wary of infinite loops towards the end).

	// NOTE: maybe the reason two_and_a_quarter works better than sqrt(5) is different:
	// I'm assuming the ratio at n=i has no knowledge of the ratio at n=i-1, but that's something that it probably should have a knowledge of (otherwise the ratio would vary based on the other function).
	// This means I should probably consider trying these other variants

	std::vector<long> root_five_silver_ratio_gap_sequence_with_memory = { 2029, 904, 404, 173, 68, 29, 13, 5, 1 };
	// This is my best version so far. Algorithm: n_i = next_silver_integer(floor(sqrt(5)*n_i-1))
}