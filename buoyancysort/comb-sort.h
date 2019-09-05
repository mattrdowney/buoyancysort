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

	std::vector<long> three_smooth_squares =
	{
		1, 4, 9, 16, 36, 64, 81, 144, 256, 324,
		576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976,
		147456, 186624, 236196, 262144, 331776, 419904, 531441, 589824, 746496, 944784,
		1048576, 1327104, 1679616, 2125764, 2359296, 2985984, 3779136, 4194304, 4782969, 5308416,
		6718464, 8503056, 9437184, 11943936, 15116544, 16777216, 19131876, 21233664, 26873856, 34012224,
		37748736, 43046721, 47775744, 60466176, 67108864, 76527504, 84934656, 107495424, 136048896, 150994944
	};

	// (Using a 1:1 ratio of squared to unsquared; if you use a 1:2 or 1:3 ratio performance improves significantly)
	// Performance on 100 elements: Phases : 1
	// Performance on 1,000 elements: Phases : 2
	// Performance on 10,000 elements: Phases: 1
	// Performance on 100,000 elements: Phases : 2 (2*29=> 58, without insertion sorts), 
	// Performance on 1,000,000 elements: Phases : 3 (3*40 => 120n, without the insertion sorts), compare that to lg(1,000,000)^2 or 397.267426
	// Performance on 10,000,000 elements: Phases : 4 (4*53 => 212n, without the insertion sorts), compare that to lg(10,000,000)^2 or 540.725107
	// Since I've started really testing (including the 100M test on the previous version), I've grown more skeptical of performance.

	// Using the 1:2 ratio only one phase is required up until 10,000,000 elements.
	// It would be more impresive if that didn't include almost all of the original sequence. (so it's basically Pratt's sorting network without guarantees)

	std::vector<long> three_smooth_squares_plus_2 =
	{
		1, 2, 4, 9, 16, 36, 64, 81, 144, 256, 324,
		576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976,
		147456, 186624, 236196, 262144, 331776, 419904, 531441, 589824, 746496, 944784
	};

	std::vector<long> three_smooth_squares_plus_up_to_9 =
	{
		1, 2, 3, 4, 6, 8, 9, 16, 36, 64, 81, 144, 256, 324,
		576, 729, 1024, 1296, 2304, 2916, 4096, 5184, 6561, 9216,
		11664, 16384, 20736, 26244, 36864, 46656, 59049, 65536, 82944, 104976,
		147456, 186624, 236196, 262144, 331776, 419904, 531441, 589824, 746496, 944784
	};

	// I think there's a serious case for interpolating between Pratt's original sorting network and the squared 3-smooth number version depending on the size of N.
	// I would think there's a way to do that on the hardware side, although most of these ideas would have a significant overhead.
}