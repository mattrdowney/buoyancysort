#pragma once

namespace CombSort
{
	template <typename Type>
	void broadphase(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
		{
			long gap = gap_sequence[gap_index];
			for (long unsorted_cursor = (before_first + 1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
			{
				int a;
				long insertion_cursor;
				if (data[unsorted_cursor - gap] > data[unsorted_cursor])
				{
					std::swap(data[unsorted_cursor - gap], data[unsorted_cursor]);
				}
			}
		}
	}

	template <typename Type>
	void not_combsort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		int phases = 0;
		while (!Sorted::verify(data, before_first, after_last))
		{
			InsertionSort::sort_runs<Type, 36>(data, before_first, after_last);
			broadphase(data, before_first, after_last, gap_sequence);
			phases += 1;
		}
		std::cout << "Phases: " << phases << std::endl;
	}
	// When I ran this it took 35 phases (just under 2^2*3^2). Sure that could be a coincidence, but I might have just found a candidate for a O(36*lg(n)) parallel time sorting algorithm.
	// Is this practical? Definitely not (at least not yet).
}