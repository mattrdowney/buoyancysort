#pragma once

#include <algorithm>
#include <stddef.h>

namespace BubbleSort
{
	template <typename Type>
	void rightward_pass(Type *data, std::size_t first, std::size_t after_last)
	{
		std::size_t cursor = first + 1;
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
	void leftward_pass(Type *data, std::size_t first, std::size_t after_last)
	{
		std::size_t cursor = 1;
		while (first + cursor < after_last)
		{
			// inversion detected
			if (data[(after_last - 1) - cursor] > data[(after_last - 1) - cursor + 1])
			{
				std::swap(data[(after_last - 1) - cursor], data[(after_last - 1) - cursor + 1]);
			}
			cursor += 1;
		}
	}
}