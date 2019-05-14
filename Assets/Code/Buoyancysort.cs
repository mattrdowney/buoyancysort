using System.Collections.Generic;

public static class Buoyancysort
{
    /// <summary>
    /// 
    /// </summary>
    /// <pseudocode>
    /// Buoyancysort(array)
    ///     pivot = Hoare-Partition()
    ///     if (iteration is even)
    ///     {
    ///         // sort left
    ///         Build-Min-Heap(array[1...pivot-1])
    ///         for left_iterator = [2...pivot-1]
    ///             if array[left_iterator - 1] is less than array[left_iterator]
    ///                 break
    ///         first_unsorted = 2;
    ///         if (left_iterator is less than or equal to pivot - 1)
    ///         {
    ///             first_known_inversion = left_iterator
    ///             minimum_inversion = array[first_known_inversion]
    ///             index_of_minimum_inversion = first_known_inversion
    ///             for left_iterator = [left_iterator...minimum(2*first_known_inversion, pivot-1)]
    ///                 if (minimum_inversion is less than array[left_iterator])
    ///                 {
    ///                     index_of_minimum_inversion = left_iterator
    ///                     minimum_inversion = array[left_iterator]
    ///                 }
    ///             first_actual_inversion = lower_bound(array, 1, first_known_inversion, minimum_inversion) // e.g. lower_bound() ~= binary_search()
    ///             swap(array, first_actual_inversion, index_of_minimum_inversion)
    ///             first_unsorted = first_actual_inversion + 1
    ///         }
    ///         else if the whole list was already sorted
    ///         {
    ///             first_unsorted = pivot
    ///         }
    ///         Buoyancysort(array[first_unsorted...pivot-1]);
    ///         
    ///         // sort right
    ///         Build-Reversed-Max-Heap(array[pivot+1...n])
    ///         while ()
    ///         {
    ///         }
    ///         Buoyancysort(array[pivot+1...n-1]);
    ///     }
    ///     else if (iteration is odd)
    ///     {
    ///         Build-Reversed-Max-Heap(array[1...pivot-1])
    ///         Build-Min-Heap(array[pivot+1...n])
    ///         //
    ///         Buoyancysort(array[1...pivot-2]);
    ///         Buoyancysort(array[pivot+2...n]);
    ///     }
    ///     
    /// </pseudocode>
    /// <param name="mutable_array"></param>
    public static sort(ref List<int> mutable_array, SortStage stage = SortStage.Min, int iterations = 6)
    {
        // if remaining size of array is n <= 64, Insertion-Sort and return
        // if iterations <= 0 use Timsort(remaining range) and return
        // if (stage == SortStage.Min)
        //{
        //    Build-Reversed-Min-Leonardo-Heap();
        //    Do Lazy-Insertion-Sort() while adaptive performance is O(k*log(log(k))) or you reach a boundary for the next Leonardo heap
        //    Either go to 2n times the number of elements sorted in this heap or the next Leonardo heap root (whichever is smaller)
        //    Bubblesort back (for one iteration) that element until it reaches the sorted bound.
        //    When you get to the sorted bound, do insertion sort on the sorted sequence.
        //    The final location of that node has all prior elements sorted (and therefore those elements can be ignored).
        //    Pick partition from median of medians (9 total elements) spanning 4n/16::n/16::12n/16 (4n/16, 5n/16, 6n/16 ... 10n/16, 11n/16, 12n/16)
        //    Find already partitioned elements in the left and right using linear search
        //    Do a Semi-Stable-Partition() of unpartitioned elements
        //    buoyancysort(first_unsorted_left...pivot-1, SortStage.Median, iterations-1)
        //    buoyancysort(pivot+1...n, SortStage.Median, iterations-1)
        //}
        //else if (stage == SortStage.Max)
        //{
        //    Build-Forward-Max-Leonardo-Heap();
        //    Do Lazy-Insertion-Sort() backwards from end while adaptive performance is O(k*log(log(k))) or you reach a boundary for the next Leonardo heap
        //    Either go to 2n times the number of elements sorted in this heap or the next Leonardo heap root (whichever is smaller)
        //    Bubblesort towards end (for one iteration) until the max element reaches the sorted bound.
        //    When you get to the sorted bound, do insertion sort on the sorted sequence.
        //    The final location of that node has all prior elements sorted (and therefore those elements can be ignored).
        //    Pick partition from median of medians (9 total elements) spanning 4n/16::n/16::12n/16 (4n/16, 5n/16, 6n/16 ... 10n/16, 11n/16, 12n/16)
        //    Find already partitioned elements in the left and right using linear search
        //    Do a Semi-Stable-Partition() of unpartitioned elements
        //    buoyancysort(1...pivot-1, SortStage.Min, iterations-1)
        //    buoyancysort(pivot+1...first_unsorted_right, SortStage.Min, iterations-1)
        //}
        //else if (stage == SortStage.Median)
        //{
        //    Pick partition from median of medians (9 total elements) spanning 4n/16::n/16::12n/16 (4n/16, 5n/16, 6n/16 ... 10n/16, 11n/16, 12n/16)
        //    Find already partitioned elements in the left and right using linear search
        //    Count elements above, below, and equal to partition line / pivot
        //    Swap pivot with middle element (elements_above + elements_equal_to/2)
        //    if pivot invalidates already_partitioned elements, do a quick fix
        //    Do a Semi-Stable-Partition() of unpartitioned elements
        //    Build-Forward-Max-Interlaced-and-Ternary-Median-Leonardo-Heap(1...pivot-1);
        //    Build-Reversed-Min-Interlaced-and-Ternary-Median-Leonardo-Heap(pivot+1...n);
        //    buoyancysort(1...pivot-2, SortStage.Max, iterations-1)
        //    buoyancysort(pivot+2...first_unsorted_right, SortStage.Max, iterations-1)
        //}
    }

    
    private enum SortStage { Min, Median, Max}
}

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.