#ifndef TEXT_FUNCTIONS_CPP
#define TEXT_FUNCTIONS_CPP
#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include "text_functions.h"
#include <cctype>

unsigned int edit_distance(const std::string& s1, const std::string& s2)
{
	const std::size_t len1 = s1.size(), len2 = s2.size();
	std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

	d[0][0] = 0;
	for (unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
	for (unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

	for (unsigned int i = 1; i <= len1; ++i)
		for (unsigned int j = 1; j <= len2; ++j)
			// note that std::min({arg1, arg2, arg3}) works only in C++11,
			// for C++98 use std::min(std::min(arg1, arg2), arg3)
			d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
	return d[len1][len2];
}

template <typename T>
std::string textJoin(std::vector<T> vectorList)
{
	std::stringstream vectorStream;
	for (int v = 0; v < vectorList.size(); v++)
	{
		vectorStream << vectorList[v];
	}
	return vectorStream.str();
}

std::string stringJoin(std::vector<std::string> strVec, char delimiter)
{
	//Export string for the joined vector
	std::string exportStr;

	//Loops through all of the elements in the vector
	for (int nVecElem = 0; nVecElem < strVec.size(); nVecElem++)
	{
		//Checks if we're on the last element
		if ((nVecElem + 1) == strVec.size())
		{
			//If we're on the last element, adds the element with no delimiter
			exportStr += strVec[nVecElem];
		}
		else
		{
			//Otherwise, adds delimiter at the end
			exportStr += strVec[nVecElem] + delimiter;
		}
	}

	return exportStr;
}

std::vector<std::string> stringSplit(std::string inputStr, char delimiter)
{
	//Export vector that will be returned by the function
	std::vector<std::string> strVec;

	//Current line string for the 
	std::string strLine = "";

	//Iterates through each of the characters in the input string
	for (int nChar = 0; nChar < static_cast<int>(inputStr.size()); nChar++)
	{
		//Current character
		char strChar = inputStr[nChar];
		
		//Checks if the current character is the delimiter
		if (strChar != delimiter)
		{
			//Is not the delimiter. Adds character to the strLine
			strLine += strChar;
		}
		else
		{
			//Is the delimiter. Adds the strLine to the strVec object
			strVec.push_back(strLine);
			strLine = "";
		}
	}
	//Because the final line will not have a delimiter, adds the current line in the queue to the export object
	if (strLine != "")
	{
		strVec.push_back(strLine);
	}
	

	return strVec;

}

//Replaces all instances of "str" in "from", to "to"
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

//Capitalizes the first letter of the input string
std::string capStr(std::string strInput) {
	std::string exportStr = strInput;
	exportStr[0] = toupper(exportStr[0]);
	return exportStr;
}

//Capitalizes all the letters of the input string
std::string allCaps(std::string strInput) {
	std::string exportStr = strInput;

	for (int c = 0; c < exportStr.size(); c++) {
		exportStr[c] = toupper(exportStr[c]);
	}
	return exportStr;
}

#endif // !TEXT_FUNCTIONS_CPP
