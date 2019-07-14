#include "stdafx.h"
#include <iostream>
#include "buoyancysort.h"
#include "bubble-sort.h"
#include "insertion-sort.h"
#include "hoare-partition.h"
#include "median-of-medians.h"

int main()
{
	int data[]{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for (int datum : data)
	{
		std::cout << datum << " ";
	}

	std::cout << std::endl;

	//medianplex3<int, 13> (approximating phi)
	//medianplex5<int, 8> (approximating phi)
	std::cout << MedianOfMedians::medianplex5<int, 13>((int*)data, (std::size_t)(sizeof(data) / sizeof(data[0]))) << std::endl;
	for (int datum : data)
	{
		std::cout << datum << " ";
	}
	char word;
	std::cin >> word;
	return 0;
}