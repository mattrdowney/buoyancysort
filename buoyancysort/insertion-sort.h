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
	void sort(Type *data, std::size_t first, std::size_t after_last)
	{
		for (std::size_t cursor = 1; cursor < after_last - first; cursor += 1)
		{
			InsertionSort::insert_from_left(data, (after_last - 1) - cursor, after_last);
		}
	}

	/// <summary>
	/// Mutator - Sorts values in range after_last-1 --> first (reverse direction) using INSERTION-SORT algorithm but gives up if too many inversions are encountered.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::lazy_leftward_sort(data)"/> representing the element at the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::lazy_leftward_sort(data)"/> representing the element one-past the end of the range to be sorted.</param>
	/// <returns>The number of elements that were sorted.</returns>
	template <typename Type>
	std::size_t lazy_leftward_sort(Type *data, std::size_t first, std::size_t after_last, double (*budget_heuristic_function)(std::size_t))
	{
		const double starting_inversion_budget = 6; // log_2(64)==6
		double budget_allowance = starting_inversion_budget;
		std::size_t budget_milestone = 64;

		std::size_t unsorted_index = 1;
		while (unsorted_index < after_last)
		{
			inversion_budget_exceeded -= InsertionSort::insert_from_left(data, (after_last - 1) - unsorted_index, after_last);
			inversion_budget_exceeded += budget_allowance;
			if (inversion_budget_exceeded < 0)
			{
				unsorted_index += 1;
				break;
			}

			if (unsorted_index >= budget_milestone)
			{
				// amortize the budget across the next n elements encountered (64, 128, 256...)
				budget_allowance = (budget_heuristic_function(budget_milestone * 2) - budget_heuristic_function(budget_milestone)) / budget_milestone;
				budget_milestone *= 2;
			}
		}
		return unsorted_index;
	}

	/// <summary>
	/// Mutator - Sorts values in range first --> after_last-1 (forward direction) using INSERTION-SORT algorithm but gives up if too many inversions are encountered.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::lazy_rightward_sort(data)"/> representing the element at the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::lazy_rightward_sort(data)"/> representing the element one-past the end of the range to be sorted.</param>
	/// <returns>The number of elements that were sorted.</returns>
	template <typename Type>
	std::size_t lazy_rightward_sort(Type *data, std::size_t first, std::size_t after_last, double(*budget_heuristic_function)(std::size_t))
	{
		const double starting_inversion_budget = 6; // log_2(64)==6
		double budget_allowance = starting_inversion_budget;
		std::size_t budget_milestone = 64;

		std::size_t unsorted_index = 1;
		while (unsorted_index < after_last)
		{
			inversion_budget_exceeded -= InsertionSort::insert_from_right(data, 0, unsorted_index);
			inversion_budget_exceeded += budget_allowance;
			if (inversion_budget_exceeded < 0)
			{
				unsorted_index += 1;
				break;
			}

			if (unsorted_index >= budget_milestone)
			{
				// amortize the budget across the next n elements encountered (64, 128, 256...)
				budget_allowance = (budget_heuristic_function(budget_milestone * 2) - budget_heuristic_function(budget_milestone)) / budget_milestone;
				budget_milestone *= 2;
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
	/// <returns>The number of inversions that were removed (i.e. the number of sequence shifts necessary to insert).</returns>
	template <typename Type>
	std::size_t insert_from_right(Type *data, std::size_t first, std::size_t rightmost_to_insert)
	{
		Type unsorted_value = data[rightmost_to_insert];
		std::size_t cursor = 1;
		while (first + cursor <= rightmost_to_insert && data[rightmost_to_insert - cursor] > unsorted_value)
		{
			data[rightmost_to_insert - cursor + 1] = data[rightmost_to_insert - cursor];
			cursor += 1;
		}
		data[rightmost_to_insert - cursor + 1] = unsorted_value;
		return cursor - 1;
	}

	/// <summary>
	/// Mutator - Sorts the value at rightmost_to_insert into the sorted range [first, rightmost_to_insert) using the left-to-right INSERTION algorithm.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="leftmost_to_insert">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element that needs to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element one-past the end of the sorted range.</param>
	/// <returns>The number of inversions that were removed (i.e. the number of sequence shifts necessary to insert).</returns>
	template <typename Type>
	std::size_t insert_from_left(Type *data, std::size_t leftmost_to_insert, std::size_t after_last)
	{
		Type unsorted_value = data[leftmost_to_insert];
		std::size_t cursor = leftmost_to_insert + 1;
		while (cursor < after_last && unsorted_value > data[cursor])
		{
			data[cursor - 1] = data[cursor];
			cursor += 1;
		}
		data[cursor - 1] = unsorted_value;
		return (cursor - 1) - leftmost_to_insert;
	}
}