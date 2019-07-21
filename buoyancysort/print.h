#pragma once

#include <iostream>

namespace Print
{
	template <typename Type>
	void print(Type *data, long before_first, long after_last)
	{
		std::cout << std::endl;
		for (int index = before_first + 1; index < after_last; index += 1)
		{
			std::cout << data[index] << " ";
		}
		std::cout << std::endl;
	}
}