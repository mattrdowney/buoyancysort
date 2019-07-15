#pragma once

#include <stddef.h>

namespace Heap
{
	/// (+1)-based -- if I'm not mistaken this is also (+0)-based indexing (which itself is going to be better-suited, ignoring the underflow problem)
	std::size_t forward_parent(std::size_t child, std::size_t size)
	{
		return child / 2;
	}

	/// (+1)-based
	std::size_t forward_left_child(std::size_t parent, std::size_t size)
	{
		return parent * 2;
	}

	/// (+1)-based
	std::size_t forward_right_child(std::size_t parent, std::size_t size)
	{
		return parent * 2 + 1;
	}
	/* // I am going to leave these un-implemented for now because there should be near-symmetry (not certain because of left/right child definitions)
	/// (-1)-based
	std::size_t reverse_parent(std::size_t child, std::size_t size)
	{
		return (size-1) - ((size-1) - child) / 2;
	}

	/// (-1)-based
	std::size_t reverse_left_child(std::size_t parent, std::size_t size)
	{
		return parent * 2;
	}

	/// (-1)-based
	std::size_t reverse_right_child(std::size_t parent, std::size_t size)
	{
		return parent * 2 + 1;
	}
	*/
	// Notably un-implemented: +0 and -0 based indexing, median heap (-1/0/+1 indexing)
}