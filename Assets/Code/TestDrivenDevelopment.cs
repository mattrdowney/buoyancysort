using System.Collections.Generic;
using UnityEngine;

public class TestDrivenDevelopment : MonoBehaviour
{
    // Start is called before the first frame update
    private void Start()
    {
        int test_cases = 10; //1000;
        int array_size = 100000; // 10'000'000
        
        // Correctness Test

        List<int> sorting_array = new List<int>(array_size);
        List<int> sorting_array2 = new List<int>(array_size);
        for (int test_number = 0; test_number < test_cases; test_number += 1)
        {
            int control_seed = test_number;
            for (int array_index = 0; array_index < array_size; array_index += 1)
            {
                int random_element = ...
                sorting_array[array_index] = random_element;
                sorting_array2[array_index] = random_element;
            }
            Buoyancysort.sort(ref sorting_array);
            sorting_array2.Sort();
            for (int array_index = 0; array_index < array_size; array_index += 1)
            {
                if (sorting_array[array_index] != sorting_array2[array_index])
                {
                    // Something is wrong with my sort
                    Debug.LogError("You fucked up: array[" + array_index + "] = (" + sorting_array[array_index] + ", " + sorting_array2[array_index] + ")");
                }
            }
        }

        // Speed test!

        // Test Buoyancysort
        sorting_array = new List<int>(array_size);
        for (int test_number = 0; test_number < test_cases; test_number += 1)
        {
            int test_seed = test_number;
            for (int array_index = 0; array_index < array_size; array_index += 1)
            {
                int random_element = ...
                sorting_array[array_index] = random_element;
            }
            // my code - STOPWATCH START
            Buoyancysort(ref sorting_array);
            // my code -  STOPWATCH STOP
        }

        // Test built-in C# sort (if it's a stable algorithm, then this is an unfair comparison).
        sorting_array = new List<int>(array_size);
        for (int control_number = 0; control_number < test_cases; control_number += 1)
        {
            int control_seed = control_number;
            for (int array_index = 0; array_index < array_size; array_index += 1)
            {

            }
            // my code - STOPWATCH START
            sorting_array.Sort();
            // my code -  STOPWATCH STOP
        }
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