#pragma once

#include <stddef.h>
#include "power-of-two.h"

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

	long long sibling(long long child, long long center)
	{
		// I can't think of how to remove branch prediction (the compiler is optimizing my code, but it doesn't seem to remove comparisons entirely)
		if (center - child > 1)
		{
			return child + ((center - child) % 2 == 0 ? -1 : +1);
		}
		if (center - child >= -1)
		{
			return -2 * (child - center) + child;
		}
		return child + ((child - center) % 2 == 0 ? +1 : -1);
	}

	template <typename Type>
	void build_max_strip(Type *data, std::size_t first, std::size_t after_last, std::size_t build_index) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		// Base case is build_index == first (no need to swap an element with itself and you need to stop the recursion).
		if (build_index > first + 1)
		{
			std::size_t forward_build_index = (after_last - 1) - build_index;
			std::size_t forward_parent_index = Heap::parent(forward_build_index, after_last - 1);
			if ((build_index - first) % 2 == 0)
			{
				if (data[forward_build_index] > data[forward_parent_index])
				{
					std::swap(data[forward_build_index], data[forward_parent_index]);
				}
				forward_build_index += 1;
				forward_parent_index += 1;
			}
			std::size_t lower_power = PowerOfTwo::lower_power_of_two(build_index - first) + first;
			while (reverse_build_index >= lower_power)
			{
				if (data[forward_build_index] > data[forward_build_index + 1])
				{
					std::swap(data[forward_build_index], data[forward_build_index + 1]);
				}
				if (data[forward_build_index + 1] > data[forward_parent_index])
				{
					std::swap(data[forward_build_index + 1], data[forward_parent_index]);
				}

				forward_build_index += 2;
				forward_parent_index += 1;
			}
			// This is often of the form 2^i-1 (where i is an integer).
			std::size_t build_index_parent = Heap::parent(build_index, first);
			// Recursively build strips e.g. 128, 64, 32, 16, 8, 4, 2, 1, 0
			// While it's not immediately apparent, cache coherency is very good.
			// Strips are essentially proccessed linearly to the root of the heap.
			build_max_strip<Type>(data, first, after_last, build_index_parent);
		}
		else if (build_index == first + 1)
		{
			if (data[after_last - 2] > data[after_last - 1])
			{
				std::swap(data[after_last - 2], data[after_last - 1]);
			}
		}
	}

	template <typename Type>
	void build_max_heap(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t build_index = after_last - 1;
		while (build_index > first)
		{
			build_max_strip<Type>(data, first, after_last, build_index);
			std::size_t lower_power = PowerOfTwo::lower_power_of_two(build_index - first);
			if (build_index == lower_power + first)
			{
				// E.g. lower_power(256) == 256, but we actually want 128 
				build_index = lower_power / 2 + first;
			}
			else
			{
				build_index = lower_power + first;
			}
			for (int i = first; i < after_last; i += 1)
			{
				std::cout << data[i] << " ";
			}
			std::cout << std::endl << std::endl;
		}
	}

	template <typename Type>
	void build_min_strip(Type *data, std::size_t first, std::size_t after_last, std::size_t build_index) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		// Base case is build_index == first (no need to swap an element with itself and you need to stop the recursion).
		if (build_index > first + 1)
		{
			std::size_t reverse_build_index = first + build_index;
			std::size_t reverse_parent_index = Heap::parent(reverse_build_index, first);
			if ((build_index - first) % 2 == 0)
			{
				if (data[reverse_parent_index] > data[reverse_build_index])
				{
					std::swap(data[reverse_parent_index], data[reverse_build_index]);
				}
				reverse_build_index -= 1;
				reverse_parent_index -= 1;
			}
			std::size_t lower_power = PowerOfTwo::lower_power_of_two(build_index - first) + first;
			while (reverse_build_index >= lower_power)
			{
				if (data[reverse_build_index - 1] > data[reverse_build_index])
				{
					std::swap(data[reverse_build_index - 1], data[reverse_build_index]);
				}
				if (data[reverse_parent_index] > data[reverse_build_index - 1])
				{
					std::swap(data[reverse_parent_index], data[reverse_build_index - 1]);
				}
				reverse_build_index -= 2;
				reverse_parent_index -= 1;
			}
			// This is often of the form 2^i-1 (where i is an integer).
			std::size_t build_index_parent = Heap::parent(build_index, first);
			// Recursively build strips e.g. 128, 64, 32, 16, 8, 4, 2, 1, 0
			// While it's not immediately apparent, cache coherency is very good.
			// Strips are essentially proccessed linearly to the root of the heap.
			build_min_strip<Type>(data, first, after_last, build_index_parent);
		}
		else if (build_index == first + 1)
		{
			if (data[first] > data[first + 1])
			{
				std::swap(data[first], data[first + 1]);
			}
		}
	}

	template <typename Type>
	void build_min_heap(Type *data, std::size_t first, std::size_t after_last) // FIXME: buoyancysort is broken until this maintains both heap properties
	{
		std::size_t build_index = after_last - 1;
		while (build_index > first)
		{
			build_min_strip<Type>(data, first, after_last, build_index);
			std::size_t lower_power = PowerOfTwo::lower_power_of_two(build_index - first);
			if (build_index == lower_power + first)
			{
				// E.g. lower_power(256) == 256, but we actually want 128 
				build_index = lower_power / 2 + first;
			}
			else
			{
				build_index = lower_power + first;
			}
			for (int i = first; i < after_last; i += 1)
			{
				std::cout << data[i] << " ";
			}
			std::cout << std::endl << std::endl;
		}
	}
}