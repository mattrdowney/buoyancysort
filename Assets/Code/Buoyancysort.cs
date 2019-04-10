using System.Collections.Generic;

public static class Buoyancysort
{
    // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 999, 999, 999, 999, 999, 999, 999, 999, 5



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
    public static sort(ref List<int> mutable_array)
    {
    }
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