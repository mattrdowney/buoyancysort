#pragma once
#include <stddef.h>
#include "insertion-sort.h"

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
		// Step 0: InsertionSort on chunk sizes 9/7 -- alternating (I have long seen k=8 as the approximate optimal crossover point for quicksort): https://cs.stackexchange.com/questions/37956/why-is-the-optimal-cut-off-for-switching-from-quicksort-to-insertion-sort-machin
		// Step 1: partition
		// Step 2: median5
		// Step 3: partition
		// Step 4: median3
		// Step 5: partition
		// Step 6: median5
		// Step 7: partition
		// Step 8: median7
		// Step 5: repeat from "Step 1"

		// Big *Asterisk: I am not really planning on implementing median of medians, unless it is in the context of shattering asymptotic running time. (eh, but it's probably important for shattering asymptotic complexity, if that's even possible)
		// also "minimize y = 2g(g-1)/(g-3), g > 3" yields 3 + sqrt(6) or ~5.45 (if you are trying to minimize comparisons)
		// trying to look at the best case instead of the worst case you get the formula: y = 1/(1-(1/2+1/c))/c*ceil(lg2(c!)), c= 3,4,5... (optimal is 5, then 4 and 6 are tied) [I previously forgot the "1-" and should have used common sense, but didn't]
		// That being said, if you use the more proper equation for insertion sort you get: y = 1/(1-(1/2+1/c))/c*(c*(c-1)/2), c=3,4,5... (which works better with smaller numbers like 3 (in theory, probably not practice))
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
	/// Mutator - 
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="run_size">The run size of small groups that are INSERTION-SORT'ed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int run_size>
	std::size_t medianplex(Type *data, std::size_t first, std::size_t after_last)
	{
		// if n <= e.g. 13 then INSERTION-SORT and return median
		std::size_t size = after_last - first;
		if (size <= run_size)
		{
			InsertionSort::sort<Type>(data, first, after_last);
			return first + size/2;
		}
		// This code is good about cache-coherency.

		// if n % 6 == 0/2/4 aren't possible because there should not be any even numbers
		// if n % 6 == 3 then <n/3><n/3><n/3>
		// if n % 6 == 5 then <n/3><n/3+2><n/3>
		// if n % 6 == 1 then <n/3+1><n/3-1><n/3+1>

		std::size_t remainder = size % 6;
		std::size_t one_third = first + size/3;
		std::size_t two_thirds = one_third + size/3;
		if (remainder == 5)
		{
			two_thirds += 2;
		}
		else if (remainder == 1)
		{
			one_third += 1;
		}
		
		std::size_t first_median = medianplex<Type, run_size>(data, first, one_third);
		std::size_t second_median = medianplex<Type, run_size>(data, one_third, two_thirds);
		std::size_t third_median = medianplex<Type, run_size>(data, two_thirds, after_last);

		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}
		if (data[second_median] > data[third_median])
		{
			std::swap(data[second_median], data[third_median]);
		}
		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}

		return second_median;
		// TODO: think about Collatz_conjecture with respect to this algorithm
	}
	template <typename Type, int run_size>
	std::size_t medianplex(Type *data, std::size_t size)
	{
		std::size_t result;
		bool even_size = (size % 2 == 0);
		if (even_size)
		{
			// you can't get a true median of even numbers (this can only happen once at the very beginning so it is in the initial calling function)
			result = medianplex<Type, run_size>(data, 0, size - 1);
			// Removing this element prevents it from being sorted, you want to swap it if necessary at the very end
			if (data[size - 2] > data[size - 1])
			{
				std::swap(data[size - 2], data[size - 1]);
			}
		}
		else
		{
			result = medianplex<Type, run_size>(data, 0, size);
		}
		return result;
	}
}