#pragma once

#include <stddef.h>

namespace Heap
{
	/// -1/0/+1 - based
	long long parent(long long child, long long center)
	{
		return (child - center) / 2 + center;
	}

	/// -1/0/+1 - based
	long long left_child(long long parent, long long center)
	{
		return (parent - center) * 2 - (parent <= center) + center;
	}

	/// -1/0/+1 - based
	long long right_child(long long parent, long long center)
	{
		return (parent - center) * 2 + (parent >= center) + center;
	}
}