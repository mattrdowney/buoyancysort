#pragma once
#include <stddef.h>

namespace MedianOfMedians
{

	/// <summary>
	/// Mutator - Sorts values in range [first, after_last) using INSERTION-SORT algorithm. No reverse variant necessary.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="insertion_sort_group_size">The number of iterations that are skipped by using INSERTION-SORT on a range of tuple_size*insertion_sort_group_size values.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing the "median of medians", which approximates the true median.</returns>
	template <typename Type, int tuple_size, int insertion_sort_group_size>
	std::size_t partition(Type *data, std::size_t first, std::size_t after_last)
	{
		int insertion_sort_size = tuple_size*insertion_sort_group_size;


		// TODO: RESEARCH: keep heap structure on both sides.
		//     alternate on build-min-heap and build-max-heap calls (using the recursive variant, not iterative)
		//     when a element must be swapped, get the complementary_node using (approximately) `1 - n`
		//     if (complementary_node is meaningful i.e. it is not on the same side as gibberish values according to a partition)
		//         if (complementary_node invalidates it's parent)
		//             "sift-up"
		//         else if (complementary_node invalidates it's children)
		//             "sift-down"
	}
}