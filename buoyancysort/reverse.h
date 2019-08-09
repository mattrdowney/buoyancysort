#pragma once

namespace Reverse
{
	/// <summary>
	/// Mutator - Reverses values in range [first, last].
	/// </summary>
	/// <param name="Type">The type of data being reversed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="InsertionSort::sort(data)"> representing the element at the start of the range to be reversed.</param>
	/// <param name="last">An index into the <see cref="InsertionSort::sort(data)"> representing the element at the end of the range to be reversed.</param>
	template <typename Type>
	void reverse(Type *data, long first, long last)
	{
		while (first < last)
		{
			std::swap(data[first], data[last]);

			first += 1;
			last -= 1;
		}
	}
}