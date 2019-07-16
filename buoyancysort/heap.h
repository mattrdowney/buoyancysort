#pragma once

#include <stddef.h>

namespace Heap
{
	/// -1/0/+1 - based
	// (-4,  0).parent = -2
	// (-3,  0).parent = -1
	// (-2,  0).parent = -1
	// (-1,  0).parent =  0
	// ( 0,  0).parent =  0
	// (+1,  0).parent =  0
	// (+2,  0).parent = +1
	// (+3,  0).parent = +1
	// (+4,  0).parent = +2

	// ( 0, +4).parent = +2
	// (+1, +4).parent = +3
	// (+2, +4).parent = +3
	// (+3, +4).parent = +4
	// (+4, +4).parent = +4
	// (+5, +4).parent = +4
	// (+6, +4).parent = +5
	// (+7, +4).parent = +5
	// (+8, +4).parent = +6
	long long parent(long long child, long long center)
	{
		return (child - center) / 2 + center;
	}

	/// -1/0/+1 - based
	// (-4,  0).left_child = -9
	// (-3,  0).left_child = -7
	// (-2,  0).left_child = -5
	// (-1,  0).left_child = -3
	// ( 0,  0).left_child = -1
	// (+1,  0).left_child = +2
	// (+2,  0).left_child = +4
	// (+3,  0).left_child = +6
	// (+4,  0).left_child = +8

	// ( 0, +4).left_child = -5
	// (+1, +4).left_child = -3
	// (+2, +4).left_child = -1
	// (+3, +4).left_child = +1
	// (+4, +4).left_child = +3
	// (+5, +4).left_child = +6
	// (+6, +4).left_child = +8
	// (+7, +4).left_child = +10
	// (+8, +4).left_child = +12
	long long left_child(long long parent, long long center)
	{
		return (parent - center) * 2 - (parent <= center) + center;
	}

	/// -1/0/+1 - based
	// (-4,  0).right_child = -8
	// (-3,  0).right_child = -6
	// (-2,  0).right_child = -4
	// (-1,  0).right_child = -2
	// ( 0,  0).right_child = +1
	// (+1,  0).right_child = +3
	// (+2,  0).right_child = +5
	// (+3,  0).right_child = +7
	// (+4,  0).right_child = +9

	// ( 0, +4).right_child = -4
	// (+1, +4).right_child = -2
	// (+2, +4).right_child =  0
	// (+3, +4).right_child = +2
	// (+4, +4).right_child = +5
	// (+5, +4).right_child = +7
	// (+6, +4).right_child = +9
	// (+7, +4).right_child = +11
	// (+8, +4).right_child = +13
	long long right_child(long long parent, long long center)
	{
		return (parent - center) * 2 + (parent >= center) + center;
	}
}