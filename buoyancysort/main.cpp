#include "stdafx.h"
#include <iostream>
#include "buoyancysort.h"

int main()
{
	int data[]{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	for (int datum : data)
	{
		std::cout << datum << std::endl;
	}
	Buoyancysort::sort((int*)data, sizeof(data));
	for (int datum : data)
	{
		std::cout << datum << std::endl;
	}
	char word;
	std::cin >> word;
	return 0;
}