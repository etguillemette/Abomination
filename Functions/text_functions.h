#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#pragma once

#include <string>
#include <vector>


//Calculates the levensthein distance between two input strings
unsigned int edit_distance(const std::string& s1, const std::string& s2);

//Joins a vector of elements into a single string. Jesus Christ I miss the simplicity of Javascript...
template <typename T>
std::string textJoin(std::vector<std::string> vectorList);

//Splits a block of text into a 2-dimensional string vector. Useful for splitting lines.
std::vector<std::string> stringSplit(std::string inputStr, char delimiter);

//Joins a vector of strings into a single string
std::string stringJoin(std::vector<std::string> strVec, char delimiter);

//Replaces all instances of a string within a larger string
void replaceAll(std::string& str, const std::string& from, const std::string& to);

std::string capStr(std::string strInput);

std::string allCaps(std::string strInput);

#endif // !TEST_FUNCTIONS_H
