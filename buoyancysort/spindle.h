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
		// The dangerous part about the Sift calls is that the parent function is currently assuming before_first and after_last are the Heap roots.
		long size = (after_last - (before_first + 1));
		/*
		Sift::leftward_sift(data, before_first, after_last);
		Sift::rightward_sift(data, before_first + 1, after_last);
		// The following lines guarantee that the "Binary Spindle" always has an even size, which is important for its "Alignment" property.
		if ((size % 2) == 0)
		{
			before_first += 1;
			size -= 1;
		}
		after_last -= 1;
		size -= 1;
		// size=0 might cause a critical failure
		if (size <= 1)
		{
			return;
		}
		assert(((after_last - (before_first + 1)) % 2) == 0);
		*/
		InterlacedEntangledDoubleBinaryHeap::build(data, before_first, after_last);
	}
}