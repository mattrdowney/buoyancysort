#pragma once

#include <assert.h>
#include "heap.h"

namespace HeapTests // The amount of namespaces is basically parody at this point
{
	void heap_tests()
	{
	//	// Parent, unshifted
	//	assert(Heap::parent(-4,  0) == -2);
	//	assert(Heap::parent(-3,  0) == -1);
	//	assert(Heap::parent(-2,  0) == -1);
	//	assert(Heap::parent(-1,  0) ==  0);
	//	assert(Heap::parent( 0,  0) ==  0);
	//	assert(Heap::parent(+1,  0) ==  0);
	//	assert(Heap::parent(+2,  0) == +1);
	//	assert(Heap::parent(+3,  0) == +1);
	//	assert(Heap::parent(+4,  0) == +2);

	//	// Parent, shifted
	//	assert(Heap::parent( 0, +4) == +2);
	//	assert(Heap::parent(+1, +4) == +3);
	//	assert(Heap::parent(+2, +4) == +3);
	//	assert(Heap::parent(+3, +4) == +4);
	//	assert(Heap::parent(+4, +4) == +4);
	//	assert(Heap::parent(+5, +4) == +4);
	//	assert(Heap::parent(+6, +4) == +5);
	//	assert(Heap::parent(+7, +4) == +5);
	//	assert(Heap::parent(+8, +4) == +6);

	//	// Left child, unshifted
	//	assert(Heap::left_child(-4,  0) == -9);
	//	assert(Heap::left_child(-3,  0) == -7);
	//	assert(Heap::left_child(-2,  0) == -5);
	//	assert(Heap::left_child(-1,  0) == -3);
	//	assert(Heap::left_child( 0,  0) == -1);
	//	assert(Heap::left_child(+1,  0) == +2);
	//	assert(Heap::left_child(+2,  0) == +4);
	//	assert(Heap::left_child(+3,  0) == +6);
	//	assert(Heap::left_child(+4,  0) == +8);

	//	// Left child, shifted
	//	assert(Heap::left_child( 0, +4) == -5);
	//	assert(Heap::left_child(+1, +4) == -3);
	//	assert(Heap::left_child(+2, +4) == -1);
	//	assert(Heap::left_child(+3, +4) == +1);
	//	assert(Heap::left_child(+4, +4) == +3);
	//	assert(Heap::left_child(+5, +4) == +6);
	//	assert(Heap::left_child(+6, +4) == +8);
	//	assert(Heap::left_child(+7, +4) == +10);
	//	assert(Heap::left_child(+8, +4) == +12);

	//	// Right child, unshifted
	//	assert(Heap::right_child(-4,  0) == -8);
	//	assert(Heap::right_child(-3,  0) == -6);
	//	assert(Heap::right_child(-2,  0) == -4);
	//	assert(Heap::right_child(-1,  0) == -2);
	//	assert(Heap::right_child( 0,  0) == +1);
	//	assert(Heap::right_child(+1,  0) == +3);
	//	assert(Heap::right_child(+2,  0) == +5);
	//	assert(Heap::right_child(+3,  0) == +7);
	//	assert(Heap::right_child(+4,  0) == +9);

	//	// Right child, shifted
	//	assert(Heap::right_child( 0, +4) == -4);
	//	assert(Heap::right_child(+1, +4) == -2);
	//	assert(Heap::right_child(+2, +4) ==  0);
	//	assert(Heap::right_child(+3, +4) == +2);
	//	assert(Heap::right_child(+4, +4) == +5);
	//	assert(Heap::right_child(+5, +4) == +7);
	//	assert(Heap::right_child(+6, +4) == +9);
	//	assert(Heap::right_child(+7, +4) == +11);
	//	assert(Heap::right_child(+8, +4) == +13);

	//	// Sibling, unshifted
	//	assert(Heap::sibling(-4, 0) == -5);
	//	assert(Heap::sibling(-3, 0) == -2);
	//	assert(Heap::sibling(-2, 0) == -3);
	//	assert(Heap::sibling(-1, 0) == +1);
	//	assert(Heap::sibling(0, 0) == 0);
	//	assert(Heap::sibling(+1, 0) == -1);
	//	assert(Heap::sibling(+2, 0) == +3);
	//	assert(Heap::sibling(+3, 0) == +2);
	//	assert(Heap::sibling(+4, 0) == +5);

	//	// Sibling, shifted
	//	assert(Heap::sibling(0, +4) == -1);
	//	assert(Heap::sibling(+1, +4) == +2);
	//	assert(Heap::sibling(+2, +4) == +1);
	//	assert(Heap::sibling(+3, +4) == +5);
	//	assert(Heap::sibling(+4, +4) == +4);
	//	assert(Heap::sibling(+5, +4) == +3);
	//	assert(Heap::sibling(+6, +4) == +7);
	//	assert(Heap::sibling(+7, +4) == +6);
	//	assert(Heap::sibling(+8, +4) == +9);
	}
}