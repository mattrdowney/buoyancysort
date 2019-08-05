#pragma once

#include <stddef.h>
#include "insertion-sort.h"

namespace MedianOfMedians
{
	/// <summary>
	/// Mutator - Finds the "median of medians", an approximation of the true median.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing the "median of medians", which approximates the true median.</returns>
	template <typename Type, int tuple_size>
	std::size_t median_of_medians(Type *data, std::size_t first, std::size_t after_last)
	{

	}

	/// <summary>
	/// Mutator - 
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="run_size">The run size of small groups that are INSERTION-SORT'ed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int run_size>
	long medianplex3(Type *data, long before_first, long after_last)
	{
		// if n <= e.g. 13 then INSERTION-SORT and return median
		long size = after_last - (before_first + 1);
		if (size <= run_size)
		{
			InsertionSort::sort<Type>(data, before_first, after_last);
			return (before_first + 1) + size / 2;
		}
		else
		{
			bool even_size = (size % 2 == 0); // you have to be careful with size 0
			if (even_size)
			{
				// you can't get a true median of even numbers (this can only happen once at the very beginning)
				long result = medianplex3<Type, run_size>(data, before_first, after_last - 1);
				// Removing this element prevents it from being sorted, you want to swap it if necessary at the very end
				if (data[after_last - 2] > data[after_last - 1])
				{
					std::swap(data[after_last - 2], data[after_last - 1]);
				}
				return result;
			}
		}
		// This code is good about cache-coherency.

		// if n % 6 == 0/2/4 aren't possible because there should not be any even numbers
		// if n % 6 == 3 then <n/3><n/3><n/3>
		// if n % 6 == 5 then <n/3><n/3+2><n/3>
		// if n % 6 == 1 then <n/3+1><n/3-1><n/3+1>

		long remainder = size % 6;
		long one_third = before_first + size/3;
		long two_thirds = one_third + size/3;
		if (remainder == 5)
		{
			two_thirds += 2;
		}
		else if (remainder == 1)
		{
			one_third += 1;
		}
		
		long first_median = medianplex3<Type, run_size>(data, before_first, one_third);
		long second_median = medianplex3<Type, run_size>(data, one_third, two_thirds);
		long third_median = medianplex3<Type, run_size>(data, two_thirds, after_last);

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
	long medianplex5(Type *data, long before_first, long after_last)
	{
		// if n <= e.g. 13 then INSERTION-SORT and return median
		long size = after_last - (before_first + 1);
		if (size <= run_size)
		{
			InsertionSort::sort<Type>(data, before_first, after_last); // TODO: FIXME: be wary of off-by-ones now (I think I am omitting data)
			return (before_first + 1) + size / 2;
		}
		else
		{
			bool even_size = (size % 2 == 0); // you have to be careful with size 0
			if (even_size)
			{
				// you can't get a true median of even numbers (this can only happen once at the very beginning)
				long result = medianplex5<Type, run_size>(data, before_first, after_last - 1);
				// Removing this element prevents it from being sorted, you want to swap it if necessary at the very end
				if (data[after_last - 2] > data[after_last - 1])
				{
					std::swap(data[after_last - 2], data[after_last - 1]);
				}
				return result;
			}
		}
		// This code is good about cache-coherency.

		// if n % 10 == 0/2/4/6/8 aren't possible because there should not be any even numbers
		// if n % 10 == 5 then <n/5><n/5><n/5><n/5><n/5>
		// if n % 10 == 7 then <n/5><n/5><n/5+2><n/5><n/5>
		// if n % 10 == 9 then <n/5><n/5+2><n/5><n/5+2><n/5>
		// if n % 10 == 1 then <n/5-1><n/5+1><n/5+1><n/5+1><n/5-1>
		// if n % 10 == 3 then <n/5+1><n/5+1><n/5-1><n/5+1><n/5+1> // you can sort of see how numbers cascade into higher dimensions

		long remainder = size % 10;
		long one_fifth = before_first + size / 5;
		long two_fifths = one_fifth + size / 5;
		long three_fifths = two_fifths + size / 5;
		long four_fifths = three_fifths + size / 5;
		if (remainder == 7)
		{
			three_fifths += 2;
			four_fifths += 2;
		}
		else if (remainder == 9)
		{
			two_fifths += 2;
			three_fifths += 2;
			four_fifths += 4;
		}
		else if (remainder == 1)
		{
			one_fifth -= 1;
			three_fifths += 1;
			four_fifths += 2;
		}
		else if (remainder == 3)
		{
			one_fifth += 1;
			two_fifths += 2;
			three_fifths += 1;
			four_fifths += 2;
		}

		long first_median = medianplex5<Type, run_size>(data, before_first, one_fifth);
		long second_median = medianplex5<Type, run_size>(data, one_fifth, two_fifths);
		long third_median = medianplex5<Type, run_size>(data, two_fifths, three_fifths);
		long fourth_median = medianplex5<Type, run_size>(data, three_fifths, four_fifths);
		long fifth_median = medianplex5<Type, run_size>(data, four_fifths, after_last);

		// Find maximum element (5th position)
		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}
		if (data[second_median] > data[third_median])
		{
			std::swap(data[second_median], data[third_median]);
		}
		if (data[third_median] > data[fourth_median])
		{
			std::swap(data[third_median], data[fourth_median]);
		}
		if (data[fourth_median] > data[fifth_median])
		{
			std::swap(data[fourth_median], data[fifth_median]);
		}

		// Find 4th largest element
		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}
		if (data[second_median] > data[third_median])
		{
			std::swap(data[second_median], data[third_median]);
		}
		if (data[third_median] > data[fourth_median])
		{
			std::swap(data[third_median], data[fourth_median]);
		}

		// Find 3th largest element
		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}
		if (data[second_median] > data[third_median])
		{
			std::swap(data[second_median], data[third_median]);
		}

		// Find minimum and 2nd largest element
		if (data[first_median] > data[second_median])
		{
			std::swap(data[first_median], data[second_median]);
		}

		return third_median;
	}

	// FIXME: figure out partial C++ Template specializations or just make this work with full template specialization (probably easier and more useful)
}