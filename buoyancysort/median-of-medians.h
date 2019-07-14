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
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int run_size>
	std::size_t medianplex3(Type *data, std::size_t first, std::size_t after_last)
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
		
		std::size_t first_median = medianplex3<Type, run_size>(data, first, one_third);
		std::size_t second_median = medianplex3<Type, run_size>(data, one_third, two_thirds);
		std::size_t third_median = medianplex3<Type, run_size>(data, two_thirds, after_last);

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
	std::size_t medianplex5(Type *data, std::size_t first, std::size_t after_last)
	{
		// if n <= e.g. 13 then INSERTION-SORT and return median
		std::size_t size = after_last - first;
		if (size <= run_size)
		{
			InsertionSort::sort<Type>(data, first, after_last);
			return first + size / 2;
		}
		// This code is good about cache-coherency.

		// if n % 10 == 0/2/4/6/8 aren't possible because there should not be any even numbers
		// if n % 10 == 5 then <n/5><n/5><n/5><n/5><n/5>
		// if n % 10 == 7 then <n/5><n/5><n/5+2><n/5><n/5>
		// if n % 10 == 9 then <n/5><n/5+2><n/5><n/5+2><n/5>
		// if n % 10 == 1 then <n/5-1><n/5+1><n/5+1><n/5+1><n/5-1>
		// if n % 10 == 3 then <n/5+1><n/5+1><n/5-1><n/5+1><n/5+1> // you can sort of see how numbers cascade into higher dimensions

		std::size_t remainder = size % 10;
		std::size_t one_fifth = first + size / 5;
		std::size_t two_fifths = one_fifth + size / 5;
		std::size_t three_fifths = two_fifths + size / 5;
		std::size_t four_fifths = three_fifths + size / 5;
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

		std::size_t first_median = medianplex5<Type, run_size>(data, first, one_fifth);
		std::size_t second_median = medianplex5<Type, run_size>(data, one_fifth, two_fifths);
		std::size_t third_median = medianplex5<Type, run_size>(data, two_fifths, three_fifths);
		std::size_t fourth_median = medianplex5<Type, run_size>(data, three_fifths, four_fifths);
		std::size_t fifth_median = medianplex5<Type, run_size>(data, four_fifths, after_last);

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

	/// <summary>
	/// Mutator - 
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="run_size">The run size of small groups that are INSERTION-SORT'ed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int run_size>
	std::size_t medianplex3(Type *data, std::size_t size)
	{
		std::size_t result;
		bool even_size = (size % 2 == 0);
		if (even_size)
		{
			// you can't get a true median of even numbers (this can only happen once at the very beginning so it is in the initial calling function)
			result = medianplex3<Type, run_size>(data, 0, size - 1);
			// Removing this element prevents it from being sorted, you want to swap it if necessary at the very end
			if (data[size - 2] > data[size - 1])
			{
				std::swap(data[size - 2], data[size - 1]);
			}
		}
		else
		{
			result = medianplex3<Type, run_size>(data, 0, size);
		}
		return result;
	}

	/// <summary>
	/// Mutator - 
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="run_size">The run size of small groups that are INSERTION-SORT'ed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int run_size>
	std::size_t medianplex5(Type *data, std::size_t size)
	{
		std::size_t result;
		bool even_size = (size % 2 == 0);
		if (even_size)
		{
			// you can't get a true median of even numbers (this can only happen once at the very beginning so it is in the initial calling function)
			result = medianplex5<Type, run_size>(data, 0, size - 1);
			// Removing this element prevents it from being sorted, you want to swap it if necessary at the very end
			if (data[size - 2] > data[size - 1])
			{
				std::swap(data[size - 2], data[size - 1]);
			}
		}
		else
		{
			result = medianplex5<Type, run_size>(data, 0, size);
		}
		return result;
	}

	// FIXME: figure out partial C++ Template specializations or just make this work with full template specialization (probably easier and more useful)
}