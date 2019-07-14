#pragma once
#include <stddef.h>
#include "insertion-sort.h"

namespace MedianOfMedians
{
	/// <summary>
	/// Mutator - Sorts values in range [first, after_last) using INSERTION-SORT algorithm. No reverse variant necessary.
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="insertion_sort_group_size">The number of iterations that are skipped by using INSERTION-SORT on a range of tuple_size*insertion_sort_group_size values.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing the "median of medians", which approximates the true median.</returns>
	template <typename Type, int tuple_size, int insertion_sort_group_size>
	std::size_t pivot(Type *data, std::size_t first, std::size_t after_last)
	{
		// Currently leaning towards:
		// Step 0: InsertionSort on chunk sizes 9/7 -- alternating (I have long seen k=8 as the approximate optimal crossover point for quicksort): https://cs.stackexchange.com/questions/37956/why-is-the-optimal-cut-off-for-switching-from-quicksort-to-insertion-sort-machin
		// Step 1: partition
		// Step 2: median5
		// Step 3: partition
		// Step 4: median3
		// Step 5: partition
		// Step 6: median5
		// Step 7: partition
		// Step 8: median7
		// Step 5: repeat from "Step 1"

		// Big *Asterisk: I am not really planning on implementing median of medians, unless it is in the context of shattering asymptotic running time. (eh, but it's probably important for shattering asymptotic complexity, if that's even possible)
		// also "minimize y = 2g(g-1)/(g-3), g > 3" yields 3 + sqrt(6) or ~5.45 (if you are trying to minimize comparisons)
		// trying to look at the best case instead of the worst case you get the formula: y = 1/(1-(1/2+1/c))/c*ceil(lg2(c!)), c= 3,4,5... (optimal is 5, then 4 and 6 are tied) [I previously forgot the "1-" and should have used common sense, but didn't]
		// That being said, if you use the more proper equation for insertion sort you get: y = 1/(1-(1/2+1/c))/c*(c*(c-1)/2), c=3,4,5... (which works better with smaller numbers like 3 (in theory, probably not practice))
		// I think I am leaning towards the "Repeated Step Algorithm" (https://stackoverflow.com/questions/3908073/optimal-median-of-medians-selection-3-element-blocks-vs-5-element-blocks)
		// Thus I would use a combination of:
		// median5 -> partition -> median5 -> partition and
		// median3 -> median3 -> partition -> median3 -> median3 -> partition
		// I still have to keep a list of the indices so that I can modify data[] directly.
		// I still have to implement some extra insertion sort and partitioning algorithms.
		// InsertionSort implementation for sorting chunks of size k e.g. sorting regions of size 9 or 5
		// InsertionSort algorithm that takes a data[], indices[] (or maybe just one array) and sorts based on that.
		// Partition algorithm that takes a ...                                                 partitions ...
		// The fact that 5 and 7 are closest to the local minimum means those should be the values used in the final median of medians version (they are also prime/coprime, which is important)
		int insertion_sort_size = tuple_size*insertion_sort_group_size;
	}

	/// <summary>
	/// Mutator - 
	/// </summary>
	/// <param name="Type">The type of data being sorted.</param>
	/// <param name="tuple_size">The max number of elements sorted (e.g. 5) in each group/tuple.</param>
	/// <param name="run_size">The run size of small groups that are INSERTION-SORT'ed.</param>
	/// <param name="data">A pointer to a contiguous array of data.</param>
	/// <param name="first">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element at the start of the range to be partitioned.</param>
	/// <param name="after_last">An index into the <see cref="MedianOfMedians::partition(data)"> representing the element one-past the end of the range to be partitioned.</param>
	/// <returns>An index into the <see cref="MedianOfMedians::partition(data)"> representing an approximation of the "median of medians" (which itself is an approximation of the median).</returns>
	template <typename Type, int tuple_size, int run_size>
	std::size_t medianplex(Type *data, std::size_t first, std::size_t after_last)
	{
		// use a stack and store pairs of (index, sort_group) <-- this is the less-hacky, more elegant way of doing it.
		// linearly process the data by taking adjacent tuples of size tuple_size with matching sort_group
		// then sorting them to find a new median of sort_group+1.
		// HACK:

		// it is best to run this on tuple_size chunks and get the median of those chunks.
		// This mitigates bad luck (a simple case being merging two medians at the end)
		std::vector<Type*> upper_medians;
		for (int tuple_index = 0; tuple_index < tuple_size; tuple_index += 1)
		{
			std::vector<Type*> runs = InsertionSort::medians_of_runs<Type, run_size>(data, first, after_last); // FIXME: you need to take 1/tuple_size of sequence (although this probably isn't the final version anyway)
			while (runs.size() > 1)
			{
				runs = InsertionSort::medians_of_runs<Type, tuple_size>(runs);
			}
			upper_medians.push_back(runs[0]);
		}
		upper_medians = InsertionSort::medians_of_runs<Type, tuple_size>(upper_medians);
		return upper_medians[0] - data;

		// Ideally what I want to do is: (for medianplex of 5 -- 3 makes more sense though).
		// for e.g. 5^3 - 1 (or 124 elements)
		// <24><25><26><25><24>
		// <4><5><6><5><4><25><26*><25><4><5><6><5><4>
		// or e.g. 5^2+1 (or 26 elements)
		// <5><5><6><5><5>
		// <5><5><1><1><2><1><1><5><5>
		// or e.g. 100 elements
		// <20><20><20><20><20>
		// <4><4><4><4><4><20><20><20><20> (those 4s are not ideal at all)

		// or e.g. 100 elements (tuple_size = 3) -- I'm sort of curious if I could discern anything from the Collatz_conjecture to help with a derivation of how to split the problem.
		// <33><34><33>
		// <<11><11><11>> <<11><12><11>> <<11><11><11>>
		// <<<3><5><3>> <<3><5><3>> <<3><5><3>>>  <<<3><5><3>> <<3><6><3>> <<3><5><3>>>  <<<3><5><3>> <<3><5><3>> <<3><5><3>>>

		// Ground rules (many of which are different ways of saying the same thing):
		// For even numbers, put an even number in the center (avoid bias)
		// Always make middle number greater than left and right (avoid lopsidedness) -- is this really a good idea?
		// Always make left and right equal (avoid bias)
		// Always ensure left (or right) and center differ by at most ? (3?)
		// The only number that can be even is the center (left and right are always odd)

		// Another idea: what are the ways you combine? -- even numbers always seem to discard some information being considered (is that necessary? -- I think it is)
		// 1 -> <1>
		// 3 -> <1><1><1>
		// 5 -> <1><3><1> // special case <1>{5}
		// 7 -> <3><1><3> // special case <1>{7}
		// 9 -> <3><3><3> // special case <1>{9}
		// 11 -> <3><5><3> // special case <1>{11}
		// 13 -> <5><3><5> // special case <1>{13}
		// 15 -> <5><5><5>
		// 17 -> <5><7><5> -- you can basically continue this forever. If you get an even number you should remove one value for simplicity.

		// I'm still trying to figure out if any of this is useful...
	}
}