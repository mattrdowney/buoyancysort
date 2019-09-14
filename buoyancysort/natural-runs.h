#pragma once

namespace NaturalRuns
{
	template <typename Type>
	long forward(Type *data, long before_first, long after_last)
	{
		long sequence_cursor = before_first + 1;
		// increasing sequence that can be left alone
		while (sequence_cursor < after_last - 1 && data[sequence_cursor] <= data[sequence_cursor + 1])
		{
			sequence_cursor += 1;
		}
		return sequence_cursor;
	}

	template <typename Type>
	long direction_agnostic(Type *data, long before_first, long after_last)
	{
		long sequence_cursor = before_first + 1;
		if (data[sequence_cursor] > data[sequence_cursor + 1])
		{
			sequence_cursor += 1;
			// decreasing sequence that needs to be reversed (maintains stability)
			while (sequence_cursor < after_last - 1 && data[sequence_cursor] > data[sequence_cursor + 1])
			{
				sequence_cursor += 1;
			}
			Reverse::reverse(data, before_first + 1, sequence_cursor);
		}
		else // <=
		{
			sequence_cursor += 1;
			// increasing sequence that can be left alone
			while (sequence_cursor < after_last - 1 && data[sequence_cursor] <= data[sequence_cursor + 1])
			{
				sequence_cursor += 1;
			}
		}
		return sequence_cursor;
	}
}