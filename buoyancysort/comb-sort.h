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
		InsertionSort::sort_runs<Type, 36>(data, before_first, after_last);
		while (!Sorted::verify(data, before_first, after_last))
		{
			broadphase(data, before_first, after_last, gap_sequence);
			InsertionSort::sort_runs<Type, 36>(data, before_first, after_last);
			phases += 1;
		}
		std::cout << "Phases: " << phases << std::endl;
	}

	template <typename Type>
	void not_combsort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence1, std::vector<long> gap_sequence2)
	{
		std::set<long> gap_sequence_set(gap_sequence1.begin(), gap_sequence1.end());
		for (long gap : gap_sequence2)
		{
			if (gap_sequence_set.size() > gap_sequence1.size() * 3)
			{
				break;
			}
			gap_sequence_set.insert(gap);
		}
		std::vector<long> gap_sequence(gap_sequence_set.begin(), gap_sequence_set.end());

		std::cout << gap_sequence.size();

		not_combsort<Type>(data, before_first, after_last, gap_sequence);
	}
}