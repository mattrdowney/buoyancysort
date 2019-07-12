#include "stdafx.h"
#include <iostream>
#include "buoyancysort.h"
#include "bubble-sort.h"
#include "insertion-sort.h"
#include "hoare-partition.h"

int main()
{
	int data[]{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for (int datum : data)
	{
		std::cout << datum << " ";
	}

	std::cout << std::endl;

	HoarePartition::partition((int*)data, 0, sizeof(data)/sizeof(data[0]), 5);
	for (int datum : data)
	{
		std::cout << datum << " ";
	}
	char word;
	std::cin >> word;
	return 0;
}