#pragma once

#include <algorithm>
#include <math.h>
#include <stddef.h>
#include "bubble-sort.h"
#include "heuristics.h"
#include "hierarchysort.h"
#include "interlaced-double-binary-heap.h"
#include "median-of-medians.h"

namespace Buoyancysort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, long chunk_size)
	{
		if ((after_last - (before_first + 1)) <= 1)
		{
			return;
		}
		InterlacedDoubleBinaryHeap::build(data, before_first, after_last);
		// once you reach a O(N/lglgN) fraction of the original array
		if ((after_last - (before_first + 1)) <= chunk_size)
		{
			Hierarchysort::sort(data, before_first, after_last);
			return;
		}
		
		// For forwards and backwards directions:
		//     "Insertion Sort" while O(a_constant) to sort a region of size "first_known_inversion" 
		//     Skip to 2*"first_known_inversion"
		//     Bubble-Sort back greatest/least element to position "first_known_inversion"
		//     "Insertion-Sort" back that greatest/least element into sorted subarray
		//     After this point, all values preceding or equal to that element are sorted and can be ignored

		long first_detected_inversion = InsertionSort::lazy_rightward_sort(data, before_first, after_last, Heuristics::log_log_n);
		long inversion_finder = std::min(MinHeap::right_child(first_detected_inversion - 1, before_first) + 1, after_last); // TODO: VERIFY: especially because of +0/+1 indexing differences (e.g. off-by-ones or unsigned underflow)
		BubbleSort::leftward_pass(data, first_detected_inversion - 1, inversion_finder);
		long new_before_first = InsertionSort::insert_from_right(data, before_first, first_detected_inversion); // omitting " - 1" is intentional
		
		long last_detected_inversion = InsertionSort::lazy_leftward_sort(data, new_before_first, after_last, Heuristics::log_log_n);
		inversion_finder = std::max(MaxHeap::left_child(first_detected_inversion + 1, after_last) - 1, new_before_first); // TODO: VERIFY: especially because of +0/+1 indexing differences (e.g. off-by-ones or unsigned underflow)
		BubbleSort::rightward_pass(data, inversion_finder, last_detected_inversion + 1);
		long new_after_last = InsertionSort::insert_from_left(data, last_detected_inversion, after_last);

		// Get approximation of "median of medians"
		//medianplex3<int, 13> (approximating phi^2) -- swap between these 
		//medianplex5<int, 8> (approximating phi)
		std::size_t pivot = MedianOfMedians::medianplex3<Type, 13>(data, new_before_first, new_after_last);
		// Create a partition line
		pivot = HoarePartition::partition(data, new_before_first, new_after_last, pivot);

		// Remainder to be implemented:
		// if pivot is bad (outside of inter-quartile range i.e. middle 50%) you need to fallback to median of medians.
		// (optional) for left and right partition:
		//    While (left > e.g. O(nlglgn)) // ditto for right tail-end
		//        "Insertion-Sort" on values in the range 4/16n : 1/16n : 12/16n
		//        pivot = Hoare-Partition(1...n) with the "Insertion-Sort"ed value at ~8/16n (from prior step) as the partition line
		//        left = pivot

		Buoyancysort::sort(data, new_before_first, pivot, chunk_size);
		Buoyancysort::sort(data, pivot, new_after_last, chunk_size);
	}

	template <typename Type>
	void sort(Type *data, long before_first, long after_last)
	{
		long count = after_last - (before_first + 1);
		const long minimum_chunk_size = 5;
		long asymptotic_chunk_size = (long)(count / Heuristics::log_log_n(count));
		long unclamped_chunk_size = std::max(minimum_chunk_size, asymptotic_chunk_size);
		long chunk_size = std::min(count, unclamped_chunk_size);
		sort<Type>(data, before_first, after_last, chunk_size);
	}
}