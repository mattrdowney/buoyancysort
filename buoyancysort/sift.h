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
	void leftward_sift(Type *data, long before_first, long after_last)
	{
		long minimum_cursor = after_last - 1;
		while (before_first + 1 < minimum_cursor)
		{
			if (before_first + 2 < minimum_cursor) // at least two elements remaining
			{
				long sequence_cursor = minimum_cursor - 2;
				if (data[sequence_cursor] >= data[sequence_cursor + 1]) // NOTE: I went for the unstable version XD
				{
					// decreasing (more accurately: non-increasing) sequence that needs to be reversed
					while (before_first + 1 < sequence_cursor && data[sequence_cursor - 1] >= data[sequence_cursor])
					{
						sequence_cursor -= 1;
					}
					Reverse::reverse(data, sequence_cursor, minimum_cursor);
					// You reverse the minimum_cursor element, so you have to insert it from the opposite side
					InsertionSort::insert_from_left(data, sequence_cursor, minimum_cursor + 1);
				}
				else // <
				{
					// increasing sequence that can be left alone
					while (before_first + 1 < sequence_cursor && data[sequence_cursor - 1] < data[sequence_cursor])
					{
						sequence_cursor -= 1;
					}
					InsertionSort::insert_from_right(data, sequence_cursor - 1, minimum_cursor);
				}
				minimum_cursor = sequence_cursor;
			}
			else // only one remaining element that hasn't been compared to minimum // TODO: I think it would be simpler if I removed this case and recompared minimum_cursor
			{
				if (data[before_first + 1] > data[minimum_cursor])
				{
					std::swap(data[before_first + 1], data[minimum_cursor]);
				}
				minimum_cursor -= 1;
			}
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
	void rightward_sift(Type *data, long before_first, long after_last)
	{
		long maximum_cursor = before_first + 1;
		while (maximum_cursor < after_last - 1)
		{
			if (maximum_cursor < after_last - 2) // at least two elements remaining
			{
				long sequence_cursor = maximum_cursor + 2;
				if (data[sequence_cursor - 1] >= data[sequence_cursor]) // NOTE: I went for the unstable version XD -- I almost feel like it's more inefficient but I'll keep it for now
				{
					// decreasing (more accurately: non-increasing) sequence that needs to be reversed
					while (sequence_cursor < after_last - 1 && data[sequence_cursor] >= data[sequence_cursor + 1])
					{
						sequence_cursor += 1;
					}
					Reverse::reverse(data, maximum_cursor, sequence_cursor);
					// You reverse the minimum_cursor element, so you have to insert it from the opposite side
					InsertionSort::insert_from_right(data, maximum_cursor - 1, sequence_cursor);
				}
				else // <
				{
					// increasing sequence that can be left alone
					while (sequence_cursor < after_last - 1 && data[sequence_cursor] < data[sequence_cursor + 1])
					{
						sequence_cursor += 1;
					}
					InsertionSort::insert_from_left(data, maximum_cursor, sequence_cursor + 1);
				}
				maximum_cursor = sequence_cursor;
			}
			else // only one remaining element that hasn't been compared to maximum
			{
				if (data[maximum_cursor] > data[after_last - 1])
				{
					std::swap(data[maximum_cursor], data[after_last - 1]);
				}
				maximum_cursor += 1;
			}
		}
	}
}