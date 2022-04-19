#pragma once

namespace ShellSort
{
	template <typename Type>
	void out_of_place_sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence, long max_size = LONG_MAX)
	{
		// for all gap sequence values in descending order that are less than the array size
		//     unwind (or decompose) array into k subarrays
		//     Insertion-Sort() on k subarrays
		//     rewind (or recompose) array into 1 superarray

		// Extra optimizations:
		// No need to move elements when gap size is 1 (trivial-sounding but easy to forget)
		// Probably inefficient to move elements when size is sufficiently small (possibly <=23)
		// While there is theoretically a way to combine unwind and rewind operations to reduce copy operations, it should be cache incoherent (and probably should be avoided).

		// This function is cache-oblivious, but when the array gets small enough it should just do in-place Shellsort (since it's wasteful to copy when the whole cache line fits in memory).

		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
		{
			long gap = gap_sequence[gap_index];
			if (gap > max_size)
			{
				continue;
			}

			// Decompose single superarray into k subarrays. It's important to scan the superarray into its subarrays left-to-right in a single pass.

			// Perform an Insertion-Sort operation on each of the k subarrays.
			// An implementation note: in theory you can be smart about adaptive performance here. A lot of this implementation is based on conjecture about the average and worst-case time complexity of ShellSort.
			// You can instead use Lazy-Insertion-Sort and when some constant amount of comparisons are exceeded switch to either 1) a slower ShellSort implementation or 2) retry "from the beginning" and if that also hits the threshold again switch to the slower implementation

			// Recompose k subarrays into single superarray. It's important to scan all subarrays into their superarray left-to-right in a single pass.
		}
	}
}