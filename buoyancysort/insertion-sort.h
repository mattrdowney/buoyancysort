#pragma once
#include <stddef.h>

namespace InsertionSort
{
	/// <summary>
	/// Mutator - Sorts values in range [first, after_last) using INSERTION-SORT algorithm. A reversed implementation would have the same result.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::sort(data)"> representing the element at the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sorted.</param>
	template <typename Type>
	std::size_t sort(Type *data, std::size_t first, std::size_t after_last)
	{
		for (std::ptrdiff_t unsorted_index = after_last-2; unsorted_index >= 0; unsorted_index -= 1)
		{
			InsertionSort::insert_from_left((std::size_t)unsorted_index, after_last);
		}
	}

	/// <summary>
	/// Mutator - Sorts values in range first --> after_last-1 using INSERTION-SORT algorithm but gives up if too many inversions are encountered.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::sort(data)"> representing the element at the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sorted.</param>
	/// <example> (displaying the cost/budget of a lazy insertion sort using at most lg(n)*(n-1) work. Ideally this should be lglgn or sqrt(lglgn) instead.)
	/// 0 (2)
	/// 1.3 (3)
	/// 3 (4)
	/// 4.86 (5)
	/// 6.85 (6)
	/// 8.9 (7)
	/// 11 (8)
	/// 13.3
	/// 15.6
	/// 17.9
	/// 20.3
	/// 22.3
	/// 25.1
	/// 27.5
	/// 30 (16)
	/// 
	/// 0x1 --- 1.5
	/// 1.5x2 --- .52
	/// 2.02x4 --- .34
	/// 2.36x8 --- .26
	/// 2.62x16 --- .21
	/// 2.83x32 --- .19
	/// 3.026x64 --- .17
	/// 3.19x128 --- .15 (budget_heuristic(n))-(budget_heuristic(n/2))/(n/2) <-- approximate any function
	/// 3.34x256 --- .13
	/// 3.47x512
	/// </example>
	template <typename Type>
	std::size_t lazy_leftward_sort(Type *data, std::size_t first, std::size_t after_last, double (*budget_heuristic_function)(std::size_t))
	{
		const std::size_t starting_inversion_budget = 64;
		std::ptrdiff_t inversion_budget_exceeded = starting_inversion_budget;
		std::ptrdiff_t unsorted_index;
		for (unsorted_index = after_last - 2; unsorted_index >= 0; unsorted_index -= 1)
		{
			inversion_budget_exceeded -= InsertionSort::insert_from_left((std::size_t)unsorted_index, after_last);
			inversion_budget_exceeded += (after_last - 1) - unsorted_index;
			if (inversion_budget_exceeded < 0)
			{
				unsorted_index += 1;
				break;
			}
		}
		return unsorted_index;
	}

	/// <summary>
	/// Mutator - Sorts the value at leftmost_to_insert into the sorted range (leftmost_to_insert, after_last) using the right-to-left INSERTION algorithm.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::insert_from_right(data)"> representing the element at the start of the sorted range.</param>
	/// <param name="rightmost_to_insert">An index into the <see cref="InsertionSort::insert_from_right(data)"> representing the element that needs to be sorted.</param>
	template <typename Type>
	std::size_t insert_from_right(Type *data, std::size_t first, std::size_t rightmost_to_insert)
	{
		Type unsorted_value = data[rightmost_to_insert];
		std::ptrdiff_t cursor = rightmost_to_insert-1;
		while (cursor >= 0 && data[cursor] > unsorted_value)
		{
			data[cursor + 1] = data[cursor];
			cursor -= 1;
		}
		data[cursor + 1] = unsorted_value;
		return rightmost_to_insert - ((std::size_t)cursor + 1);
	}

	/// <summary>
	/// Mutator - Sorts the value at rightmost_to_insert into the sorted range [first, rightmost_to_insert) using the left-to-right INSERTION algorithm.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="leftmost_to_insert">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element that needs to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element one-past the end of the sorted range.</param>
	template <typename Type>
	std::size_t insert_from_left(Type *data, std::size_t leftmost_to_insert, std::size_t after_last)
	{
		Type unsorted_value = data[leftmost_to_insert];
		std::size_t cursor = leftmost_to_insert + 1;
		while (cursor < after_last && data[cursor] < unsorted_value)
		{
			data[cursor - 1] = data[cursor];
			cursor += 1;
		}
		data[cursor - 1] = unsorted_value;
		return (cursor - 1) - leftmost_to_insert;
	}
}