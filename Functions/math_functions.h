#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
#pragma once
#include <vector>


//"Wraps" an integer around to the max matrix width
int wrap(int x, int max);

//Just a test function
void testFunc(std::vector<int>& vList);

//Just another test function
void printVector(std::vector<int> vList);

//Calculates the minimum between two values
int getMin(int x, int y);

//Removes decimal plates of a float
float nFloatVal(float input, int places);

//Rounds a float to an int
int roundCents(float fInput);

#endif // !MATH_FUNCTIONS_H


