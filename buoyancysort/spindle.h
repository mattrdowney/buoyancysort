#pragma once

#include "interlaced-double-binary-heap.h"
#include "interlaced-entangled-double-binary-heap.h"

namespace Spindle
{
	// This is a "Binary Spindle" to be specific
	// As I define them, "Spindles" have the following properties:
	// "Shape" and "Heap" properties of a -1-indexed min heap and a N-indexed max heap.
	// "Interlaced" - changes to one heap affect the stability of the other heap
	// "Entangled" - siblings must be in sorted order
	// "Aligned" - the min and max heap must share sibling tuples (if they exist).
	template <typename Type>
	void build(Type *data, long before_first, long after_last)
	{
		InterlacedEntangledDoubleBinaryHeap::build(data, before_first, after_last);
	}
}