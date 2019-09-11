#pragma once
#include <stddef.h>

namespace Hierarchysort
{
	// This is what I called hard O(n) adaptive best-case performance. This is better with nearly sorted data.
	template <typename Type>
	void in_place(Type *data, long before_first, long after_last) // conceptually, this is big-endian. What I think of as in-place isn't actually in-place if I remember correctly (once I realized in-place merge costs klg(k)).
	{
		// This one has by far the coolest optimizations -in my opinion- (although I think there's a bit more overhead related to them, so you probably want to e.g. insertion sort 32 element chunks instead of 16).
	}

	// This is what I called soft O(n) adaptive best-case performance. This is better with random data (I have to test this theory, though).
	template <typename Type>
	void out_of_place(Type *data, long before_first, long after_last) // conceptually, this is little-endian
	{
		// You can do a lot of cool stuff with bit shifting. To get a VList index from a size you basically just bitshift/multiply by 2 (2 because there are 2n elements in the VList). If you need the second list just add 1 since they are interlaced.
		// 1 1 2 2 4 4 8 8 16 16 (find 16a, note how there are ~32 elements before the first 16).
		// Let's say you want to merge 16 (0b'0001'0000) elements into 240 (0b'1111'0000) elements. You bitwise AND (i.e. & operator) (0b'0001'0000) with (0b'1111'0000). Because there is a conflict, you place the elements into slot b and merge (conceptually, although you can skip the redundant copy). You can then bitshift up and notice a conflict again, etc, so merge. At the end add 16 and 240 to get 256 elements.
		// Data structure: VList (conceptually, not actually) storing power-of-two pairs of interlaced lists i.e. (a,b) represented as (a0, b0, a1, b1, a2, b2, a3, b3...)
	}

	// As a hypothetical, this could be similar to an out-of-place hierarchysort concept I was considering (based on what I remember there was some interesting zig-zagging). One idea I had was to overlay power-of-two lists (a,b) like so (a0, bn, a1, bn-1, a2, bn-2, a3, bn-3 ... an-3, b3, an-2, b2, an-1, b1, an, b0). That being said, I remember I was working on multiple concepts when I started on this, so I shouldn't lock anything into place yet since I'm essentially in the discovery stage.
	template <typename Type>
	void hybrid(Type *data, long before_first, long after_last) // conceptually this is a hybrid of big-endian and little-endian (supposed to be adaptive in both the real and programmer sense of the word). (This was a work-in-progress.)
	{
	}
}