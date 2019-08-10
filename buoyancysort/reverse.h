#pragma once

namespace Reverse
{
	/// <summary>
	/// Mutator - Reverses values in range [first, last].
	/// </summary>
	/// <param name="Type">The type of data being reversed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="Reverse::reverse(data)"> representing the element at the start of the range to be reversed.</param>
	/// <param name="last">An index into the <see cref="Reverse::reverse(data)"> representing the element at the end of the range to be reversed.</param>
	template <typename Type>
	void reverse(Type *data, long first, long last)
	{
		while (first < last)
		{
			std::swap(data[first], data[last]);

			first += 1;
			last -= 1;
		}
	}

	/// <summary>
	/// Mutator - Reverses leftwards the minimum value by reversing decreasing sequences in range (before_first, after_last).
	/// </summary>
	/// <param name="Type">The type of data being reversed left.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be reversed.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be reversed.</param>
	template <typename Type>
	void leftward_pass(Type *data, long before_first, long after_last)
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
	/// Mutator - Reverses rightwards the maximum value by reversing decreasing sequences in range (before_first, after_last).
	/// </summary>
	/// <param name="Type">The type of data being reversed right.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be reversed.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be reversed.</param>
	template <typename Type>
	void rightward_pass(Type *data, long before_first, long after_last)
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

	//sort(data, before_first, after_last) -- ReverseSort'ed
	// Originally this was intended to be a dumb sort, but I might make it smarter
	// Notable things that could be used 1) binary search 2) AB => BA by reverse(reverse(A)reverse(B))
	// Thus, this would be very similar to TimSort in that you would get a bunch of "runs" and just merge them through reversals
}