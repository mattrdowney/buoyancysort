#pragma once

#include <algorithm>
#include "insertion-sort.h"
#include "reverse.h"

namespace Sift
{
	/// <summary>
	/// Mutator - Bubblesorts leftwards the minimum value while reversing decreasing sequences in range (before_first, after_last).
	/// </summary>
	/// <param name="Type">The type of data being sifted left.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be sifted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sifted.</param>
	template <typename Type>
	void leftward_sift(Type *data, long before_first, long after_last) // Notably, this version of sift could be "worse", so I should keep that in mind.
	{
		long minimum_cursor = after_last - 1;
		while (before_first + 1 < minimum_cursor)
		{
			long sequence_cursor = minimum_cursor - 1;
			if (data[sequence_cursor] > data[sequence_cursor + 1])
			{
				// decreasing sequence that needs to be reversed
				while (before_first + 1 < sequence_cursor && data[sequence_cursor - 1] > data[sequence_cursor])
				{
					sequence_cursor -= 1;
				}
				Reverse::reverse(data, sequence_cursor, minimum_cursor);
			}
			else // <=
			{
				// increasing sequence that can be left alone
				while (before_first + 1 < sequence_cursor && data[sequence_cursor - 1] <= data[sequence_cursor])
				{
					sequence_cursor -= 1;
				}
			}
			minimum_cursor = sequence_cursor;
		}
	}

	/// <summary>
	/// Mutator - Bubblesorts rightwards the maximum value while reversing decreasing sequences in range (before_first, after_last).
	/// </summary>
	/// <param name="Type">The type of data being sifted right.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be sifted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sifted.</param>
	template <typename Type>
	void rightward_sift(Type *data, long before_first, long after_last) // Notably, this version of sift could be "worse", so I should keep that in mind.
	{
		long maximum_cursor = before_first + 1;
		while (maximum_cursor < after_last - 1)
		{
			long sequence_cursor = maximum_cursor + 1;
			if (data[sequence_cursor - 1] > data[sequence_cursor])
			{
				// decreasing (more accurately: non-increasing) sequence that needs to be reversed
				while (sequence_cursor < after_last - 1 && data[sequence_cursor] > data[sequence_cursor + 1])
				{
					sequence_cursor += 1;
				}
				Reverse::reverse(data, maximum_cursor, sequence_cursor);
			}
			else // <=
			{
				// increasing sequence that can be left alone
				while (sequence_cursor < after_last - 1 && data[sequence_cursor] <= data[sequence_cursor + 1])
				{
					sequence_cursor += 1;
				}
			}
			maximum_cursor = sequence_cursor;
		}
	}
}