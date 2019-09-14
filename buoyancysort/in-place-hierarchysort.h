#pragma once

#include <stddef.h>

namespace Hierarchysort
{
	// Let's get information theoretical.
	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. What I think of as in-place isn't actually in-place if I remember correctly (once I realized in-place merge costs klg(k)).
	{
		// This one has by far the coolest optimizations -in my opinion- (although I think there's a bit more overhead related to them, so you probably want to e.g. insertion sort 32 element chunks instead of 16).
	}
}