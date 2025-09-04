#ifndef MATH_FUNCTIONS_CPP
#define MATH_FUNCTIONS_CPP
#pragma once
#include "math_functions.h"
#include <vector>
#include <iostream>
#include <string>


int wrap(int x, int max)
{
	
	return(x % max) >= 0 ? (x % max): max+(x % max);
}


void testFunc(std::vector<int>& vList)
{

	for (int i = 0; i < static_cast<int>(vList.size()); i++)
	{
		vList[i] = vList[i] + 1;
	}

}

int getMin(int x, int y)
{
	return ((x) < (y) ? (x) : (y));
}


void printVector(std::vector<int> vList)
{
	for (int i = 0; i < static_cast<int>(vList.size()); i++)
	{
		std::cout << "Element " << i << " is " << vList[i] << "\n";
	}
}

std::string nFloatVal(float input, int places)
{
	return "Not currently working!!! Try gain later";
}


int roundCents(float fInput)
{
	return 0;
}

#endif // !math_functions
