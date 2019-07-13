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
	std::size_t pivot(Type *data, std::size_t first, std::size_t after_last)
	{
		// Big *Asterisk: I am not really planning on implementing median of medians, unless it is in the context of shattering asymptotic running time.
		// also "minimize y = 2g(g-1)/(g-3), g > 3" yields 3 + sqrt(6) or ~5.45 (if you are trying to minimize comparisons)
		// The fact that 5 and 7 are closest to the local minimum means those should be the values used in the final median of medians version (they are also prime/coprime, which is important)
		int insertion_sort_size = tuple_size*insertion_sort_group_size;
	}

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
	std::size_t medianplex(Type *data, std::size_t first, std::size_t after_last)
	{
		// use a stack and store pairs of (index, sort_group)
		// linearly process the data by taking adjacent tuples of size tuple_size with matching sort_group
		// then sorting them to find a new median of sort_group+1.
		int insertion_sort_size = tuple_size * insertion_sort_group_size;
	}
}