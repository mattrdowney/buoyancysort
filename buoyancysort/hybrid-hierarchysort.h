#pragma once

#include <stddef.h>

namespace Hierarchysort
{
	// As a hypothetical, this could be similar to an out-of-place hierarchysort concept I was considering (based on what I remember there was some interesting zig-zagging). One idea I had was to overlay power-of-two lists (a,b) like so (a0, bn, a1, bn-1, a2, bn-2, a3, bn-3 ... an-3, b3, an-2, b2, an-1, b1, an, b0). That being said, I remember I was working on multiple concepts when I started on this, so I shouldn't lock anything into place yet since I'm essentially in the discovery stage.
	template <typename Type>
	void hybrid(Type *data, long before_first, long after_last) // conceptually this is a hybrid of big-endian and little-endian (supposed to be adaptive in both the real and programmer sense of the word). (This was a work-in-progress.)
	{
		// NOTE: the biggest implementation issue here is stability.
		// If you let certain chunks enter the VList at advanced positions (not starting from 1, 2, 4, 8...) you essentially let them shortcut the line, breaking stability.
	}
}