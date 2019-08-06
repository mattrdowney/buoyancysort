#pragma once
#include <stddef.h>

namespace InsertionSort
{
	/// <summary>
	/// Mutator - Sorts values in range (before_first, after_last) using INSERTION-SORT algorithm. A reversed implementation would have the same result.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sorted.</param>
	template <typename Type>
	void sort(Type *data, long before_first, long after_last)
	{
		for (long cursor = before_first + 2; cursor < after_last; cursor += 1)
		{
			InsertionSort::insert_from_right(data, before_first, cursor);
		}
	}

	/// <summary>
	/// Mutator - Partially sorts values in range (before_first, after_last) in the subranges [0, run_size) using INSERTION-SORT algorithm. A reversed implementation would have a similar result.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="before_first">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-before the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::sort(data)"> representing the element one-past the end of the range to be sorted.</param>
	template <typename Type, long run_size>
	void sort_runs(Type *data, long before_first, long after_last)
	{
		long total_elements = after_last - (before_first + 1);
		long full_runs = total_elements / run_size;
		long full_run_elements = full_runs * run_size;
		long remainder = total_elements - full_run_elements;
		for (long run = 0; run < full_runs; run += 1)
		{
			long run_before_first = before_first + run * run_size;
			long run_after_last = run_before_first + run_size + 1;
			sort(data, run_before_first, run_after_last);
		}
		if (remainder > 1)
		{
			sort(data, after_last - remainder - 1, after_last);
		}
	}

	/// <summary>
	/// Mutator - Sorts values in range after_last-1 --> first (reverse direction) using INSERTION-SORT algorithm but gives up if too many inversions are encountered.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::lazy_leftward_sort(data)"/> representing the element one-before the start of the range to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::lazy_leftward_sort(data)"/> representing the element one-past the end of the range to be sorted.</param>
	/// <returns>The number of elements that were sorted.</returns>
	template <typename Type>
	long lazy_leftward_sort(Type *data, long before_first, long after_last, double (*budget_heuristic_function)(long))
	{
		const double starting_inversion_budget = 6; // log_2(64)==6
		double budget_allowance = starting_inversion_budget;
		long budget_milestone = 64;
		long budget_marker = 1;
		double inversion_budget_exceeded = 0;
		long unsorted_index = after_last - 2;
		while (unsorted_index > before_first)
		{
			inversion_budget_exceeded -= InsertionSort::insert_from_left(data, unsorted_index, after_last);
			inversion_budget_exceeded += budget_allowance;
			unsorted_index -= 1;
			budget_marker += 1;
			if (inversion_budget_exceeded < 0)
			{
				break;
			}
			if (budget_marker >= budget_milestone)
			{
				// amortize the budget across the next n elements encountered (64, 128, 256...)
				budget_allowance = ((budget_milestone * 2) * budget_heuristic_function(budget_milestone * 2) - (budget_milestone) * budget_heuristic_function(budget_milestone)) / budget_milestone;
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
	long lazy_rightward_sort(Type *data, long before_first, long after_last, double(*budget_heuristic_function)(long))
	{
		const double starting_inversion_budget = 6; // log_2(64)==6
		double budget_allowance = starting_inversion_budget;
		long budget_milestone = 64;
		long budget_marker = 1;
		double inversion_budget_exceeded = 0;
		long unsorted_index = before_first + 2;
		while (unsorted_index < after_last)
		{
			inversion_budget_exceeded -= InsertionSort::insert_from_right(data, before_first, unsorted_index);
			inversion_budget_exceeded += budget_allowance;
			unsorted_index += 1;
			budget_marker += 1;
			if (inversion_budget_exceeded < 0)
			{
				break;
			}
			if (budget_marker >= budget_milestone)
			{
				// amortize the budget across the next n elements encountered (64, 128, 256...)
				budget_allowance = ((budget_milestone * 2) * budget_heuristic_function(budget_milestone * 2) - (budget_milestone) * budget_heuristic_function(budget_milestone)) / budget_milestone;
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
	/// <param name="before_first">An index into the <see cref="InsertionSort::insert_from_right(data)"> representing the element one-before the start of the sorted range.</param>
	/// <param name="rightmost_to_insert">An index into the <see cref="InsertionSort::insert_from_right(data)"> representing the element that needs to be sorted.</param>
	/// <returns>The number of inversions that were removed (i.e. the number of sequence shifts necessary to insert).</returns>
	template <typename Type>
	long insert_from_right(Type *data, long before_first, long rightmost_to_insert)
	{
		Type unsorted_value = data[rightmost_to_insert];
		long cursor = rightmost_to_insert - 1;
		while (before_first < cursor && data[cursor] > unsorted_value)
		{
			data[cursor + 1] = data[cursor];
			cursor -= 1;
		}
		data[cursor + 1] = unsorted_value;
		return rightmost_to_insert - (cursor + 1);
	}

	/// <summary>
	/// Mutator - Sorts the value at rightmost_to_insert into the sorted range (before_first, rightmost_to_insert) using the left-to-right INSERTION algorithm.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="leftmost_to_insert">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element that needs to be sorted.</param>
	/// <param name="after_last">An index into the <see cref="InsertionSort::insert_from_left(data)"> representing the element one-past the end of the sorted range.</param>
	/// <returns>The number of inversions that were removed (i.e. the number of sequence shifts necessary to insert).</returns>
	template <typename Type>
	long insert_from_left(Type *data, long leftmost_to_insert, long after_last)
	{
		Type unsorted_value = data[leftmost_to_insert];
		long cursor = leftmost_to_insert + 1;
		while (cursor < after_last && unsorted_value > data[cursor])
		{
			data[cursor - 1] = data[cursor];
			cursor += 1;
		}
		data[cursor - 1] = unsorted_value;
		return (cursor - 1) - leftmost_to_insert;
	}
}