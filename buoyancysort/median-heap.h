#pragma once

#include <stddef.h>

namespace MedianHeap
{
	/// -1/0/+1 - based
	template <typename Type>
	Type parent(Type child, Type center)
	{
		return (child - center) / 2 + center;
	}

	/// -1/0/+1 - based
	template <typename Type>
	Type left_child(Type parent, Type center)
	{
		return (parent - center) * 2 - (parent <= center) + center;
	}

	/// -1/0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center)
	{
		return (parent - center) * 2 + (parent >= center) + center;
	}

	template <typename Type>
	Type sibling(Type child, Type center)
	{
		// I can't think of how to remove branch prediction (the compiler is optimizing my code, but it doesn't seem to remove comparisons entirely)
		if (center - child > 1)
		{
			return child + ((center - child) % 2 == 0 ? -1 : +1);
		}
		if (center - child >= -1)
		{
			return -2 * (child - center) + child;
		}
		return child + ((child - center) % 2 == 0 ? +1 : -1);
	}
}