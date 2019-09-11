#pragma once

namespace MergeSort
{
	// Lots of assumptions are made here. The user has to adjust the pointers manually. The user has to allocate buffers of the right sizes (e.g. output must be at least left_size + right_size or greater).
	template <typename Type>
	void merge(Type *output, Type *left_input, Type *right_input, long left_size, long right_size)
	{
		long left_cursor = 0;
		long right_cursor = 0;
		long output_cursor = 0;
		while (left_cursor < left_size && right_cursor < right_size)
		{
			if (left_input[left_cursor] <= right_input[right_cursor]) // ensures sort is stable
			{
				output[output_cursor] = left_input[left_cursor];
				left_cursor += 1;
			}
			else
			{
				output[output_cursor] = right_input[right_cursor];
				right_cursor += 1;
			}
			output_cursor += 1;
		}

		// Deal with any remainder (no need to compare anymore)
		while (left_cursor < left_size)
		{
			output[output_cursor] = left_input[left_cursor];
			left_cursor += 1;
			output_cursor += 1;
		}
		while (right_cursor < left_size)
		{
			output[output_cursor] = right_input[right_cursor];
			right_cursor += 1;
			output_cursor += 1;
		}
	}
}