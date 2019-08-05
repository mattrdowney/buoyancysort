#pragma once

#include <algorithm>
#include <stddef.h>

namespace BubbleSort
{
	template <typename Type>
	void rightward_pass(Type *data, long before_first, long after_last)
	{
		long cursor = before_first + 2;
		while (cursor < after_last)
		{
			// inversion detected
			if (data[cursor - 1] > data[cursor])
			{
				std::swap(data[cursor - 1], data[cursor]);
			}
			cursor += 1;
		}
	}

	template <typename Type>
	void leftward_pass(Type *data, long before_first, long after_last)
	{
		long cursor = after_last - 2;
		while (before_first < cursor)
		{
			// inversion detected
			if (data[cursor] > data[cursor + 1])
			{
				std::swap(data[cursor], data[cursor + 1]);
			}
			cursor -= 1;
		}
	}
}