#pragma once

#include <math.h>
#include <stddef.h>

namespace FractalMinHeap
{
	/// 0/+1 - based
	template <typename Type>
	Type parent(Type child, Type center, double tuple_size)
	{
		return floor((child - center) / tuple_size) + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type left_child(Type parent, Type center, double tuple_size)
	{
		return ceil((parent - center) * tuple_size) + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center, double tuple_size)
	{
		return ceil((parent - center) * tuple_size + (tuple_size - 1)) + center;
	}
}