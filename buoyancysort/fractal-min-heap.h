#pragma once

#include <math.h>
#include <stddef.h>

// FIXME: undefined behavior because floating-point math is imprecise and not fully specified
namespace FractalMinHeap
{
	// Trying to extend the concept of a d-ary heap: https://en.wikipedia.org/wiki/D-ary_heap

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
		return ceil((parent - center + 1) * tuple_size) + center;
	}

	/// 0/+1 - based
	template <typename Type>
	Type right_child(Type parent, Type center, double tuple_size)
	{
		return ceil((parent - center + 1) * tuple_size + (tuple_size - 1)) + center;
	}
}