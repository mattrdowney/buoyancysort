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

	std::vector<long> three_smooth_squares =
	{
		1, 4, 9, 16, 36, 64, 81, 144, 256, 324,
		576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976,
		147456, 186624, 236196, 262144, 331776, 419904, 531441, 589824, 746496, 944784
	};

	std::vector<long> three_smooth_squares_plus_2 =
	{
		1, 2, 4, 9, 16, 36, 64, 81, 144, 256, 324,
		576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976,
		147456, 186624, 236196, 262144, 331776, 419904, 531441, 589824, 746496, 944784
	};
}