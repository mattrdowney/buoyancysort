#pragma once

#include <algorithm>
#include <math.h>
#include <stddef.h>

// FIXME: undefined behavior because floating-point math is imprecise and not fully specified
namespace FractalMaxHeap
{
	/// -1/0 - based
	template <typename Type>
	Type parent(Type child, Type center, double tuple_size)
	{
		return floor((child - center) / tuple_size) + center;
	}

	/// -1/0 - based
	template <typename Type>
	Type left_child(Type parent, Type center, long tuple_size)
	{
		return ceil((parent - center + 1) * tuple_size - (tuple_size - 1)) + center;
	}

	/// -1/0 - based
	template <typename Type>
	Type right_child(Type parent, Type center, long tuple_size)
	{
		return ceil((parent - center + 1) * tuple_size) + center;
	}
}