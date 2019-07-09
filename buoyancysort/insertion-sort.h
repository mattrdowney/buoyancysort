#pragma once
#include <stddef.h>

namespace InsertionSort
{
	/// <summary>
	/// Mutator - Sorts values in range [first, after_last) using INSERTION-SORT algorithm. No reverse variant necessary.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::sort(data)"> representing the element at the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sorted.</param>
	template <typename Type>
	std::size_t sort(Type *data, std::size_t first, std::size_t after_last)
	{
		function insertionSort(array A)
			for i from 1 to length[A] - 1 do
				value : = A[i]
				j : = i - 1
				while j >= 0 and A[j] > value do
					A[j + 1] : = A[j]
					j : = j - 1
					done
					A[j + 1] = value
					done
	}

	/// <summary>
	/// Mutator - Sorts values at rightmost_to_insert into the sorted range [first, rightmost_to_insert) using the left-to-right INSERTION algorithm.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element at the start of the range to be sorted.</param>
	/// <param name="rightmost_to_insert">An index into the <see cref="MedianOfMedians::insert_from_left(data)"> representing the element one-past the end of the range to be sorted.</param>
	template <typename Type>
	std::size_t insert_from_left(Type *data, std::size_t first, std::size_t rightmost_to_insert)
	{
		Type value = data[rightmost_to_insert];
		std::size_t cursor = rightmost_to_insert-1;
		j : = i - 1
		while j >= 0 and A[j] > value do
			A[j + 1] : = A[j]
			j : = j - 1
			done
			A[j + 1] = value
			done
	}
}