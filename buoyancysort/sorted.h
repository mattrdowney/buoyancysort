#pragma once

namespace Sorted
{
	template <typename Type>
	bool verify(Type *data, long before_first, long after_last) // I should have done this a long time ago
	{
		bool sorted = true;
		for (long cursor = before_first + 1; cursor < after_last - 1; cursor += 1)
		{
			if (data[cursor] > data[cursor + 1])
			{
				sorted = false;
				break;
			}
		}
		if (!sorted)
		{
			std::cerr << "NOT sorted" << std::endl;
		}
		return sorted;
	}
}