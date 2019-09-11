#pragma once
#include <stddef.h>

namespace Hierarchysort
{
	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. What I think of as in-place isn't actually in-place if I remember correctly (once I realized in-place merge costs klg(k)).
	{
	}

	// This is what I called soft O(n) adaptive best-case performance. This is better with random data (I have to test this theory, though).
	template <typename Type>
	void out_of_place(Type *data, long before_first, long after_last) // conceptually, this is little-endian
	{
		// Data structure: VList (conceptually, not actually) storing power-of-two pairs of interlaced lists i.e. (a,b) represented as (a0, b0, a1, b1, a2, b2, a3, b3...)
	}

	template <typename Type>
	void hybrid(Type *data, long before_first, long after_last) // conceptually this is a hybrid of big-endian and little-endian. (This was a work-in-progress.)
	{
	}
}