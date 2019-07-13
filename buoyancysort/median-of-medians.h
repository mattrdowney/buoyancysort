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
		// Currently leaning towards:
		// Step 0: InsertionSort on chunk sizes 7 (I have long seen k=8 as the approximate optimal crossover point for quicksort): https://cs.stackexchange.com/questions/37956/why-is-the-optimal-cut-off-for-switching-from-quicksort-to-insertion-sort-machin
		// Step 1: partition
		// Step 2: median5 (since 7 and 5 are more dissimilar and you ignore more values)
		// Step 3: partition
		// Step 4: median3
		// Step 5: repeat from "Step 1"
		// I can't guarantee this works with my skill level, but it seems intuitive that it should work.

		// Big *Asterisk: I am not really planning on implementing median of medians, unless it is in the context of shattering asymptotic running time. (eh, but it's probably important for shattering asymptotic complexity, if that's even possible)
		// also "minimize y = 2g(g-1)/(g-3), g > 3" yields 3 + sqrt(6) or ~5.45 (if you are trying to minimize comparisons)
		// trying to look at the best case instead of the worst case you get the formula: y = 1/(1/2+1/c)/c*ceil(lg2(c!)), c = 3,5,7... (optimal is 3 by a long shot, then 5)
		// I think I am leaning towards the "Repeated Step Algorithm" (https://stackoverflow.com/questions/3908073/optimal-median-of-medians-selection-3-element-blocks-vs-5-element-blocks)
		// Thus I would use a combination of:
		// median5 -> partition -> median5 -> partition and
		// median3 -> median3 -> partition -> median3 -> median3 -> partition
		// I still have to keep a list of the indices so that I can modify data[] directly.
		// I still have to implement some extra insertion sort and partitioning algorithms.
		// InsertionSort implementation for sorting chunks of size k e.g. sorting regions of size 9 or 5
		// InsertionSort algorithm that takes a data[], indices[] (or maybe just one array) and sorts based on that.
		// Partition algorithm that takes a ...                                                 partitions ...
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