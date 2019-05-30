using System.Collections.Generic;

public static class Buoyancysort
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="mutable_array"></param>
    public static sort(ref List<int> mutable_array, SortStage stage = SortStage.Min, int iterations = 6)
    {
        // if (size is less than e.g. O(NlglgN) where N is the size of the original array)
        //     Build-Interlaced-Double-Binary-Heap(1...n)
        //     Timsort(1...n)
        //     return
        // Build-Interlaced-Double-Binary-Heap(1...n) // preferably keeping heap structure on both sides
        // For forwards and backwards directions:
        //     "Insertion Sort" while O(a_constant) to sort a region of size "first_known_inversion" 
        //     Skip to 2*"first_known_inversion"
        //     Bubble-Sort back greatest/least element to position "first_known_inversion"
        //     "Insertion-Sort" back that greatest/least element into sorted subarray
        //     After this point, all values preceding or equal to that element are sorted and can be ignored
        // "Insertion-Sort" on values in the range 4/16n : 1/16n : 12/16n
        // pivot = Hoare-Partition(1...n) with the "Insertion-Sort"ed value at ~8/16n (from prior step) as the partition line
        // if pivot is bad (outside of inter-quartile range i.e. middle 50%) you need to fallback to statistical sampling then fallback to median of medians.
        // (optional) for left and right partition:
        //    While (left > e.g. O(nlglgn)) // ditto for right tail-end
        //        "Insertion-Sort" on values in the range 4/16n : 1/16n : 12/16n
        //        pivot = Hoare-Partition(1...n) with the "Insertion-Sort"ed value at ~8/16n (from prior step) as the partition line
        //        left = pivot
        // Buoyancysort(first_unsorted_left...pivot - 1)
        // Buoyancysort(pivot + 1...first_unsorted_right)

        // define Build-Interlaced-Double-Binary-Heap()
        //     alternate on build-min-heap and build-max-heap calls (using the recursive variant, not iterative)
        //     when a element must be swapped, get the complementary_node using (approximately) `1 - n`
        //     if (complementary_node is meaningful i.e. it is not on the same side as gibberish values according to a partition)
        //         if (complementary_node invalidates it's parent)
        //             "sift-up"
        //         else if (complementary_node invalidates it's children)
        //             "sift-down"

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