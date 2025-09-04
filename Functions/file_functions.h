#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H
#pragma once

#include <string>
#include <vector>

//Exports a timestamp of the current time
std::string timestamp();

//Writes content to a file, appends if necessary
void writeFile(std::string content, std::string path, bool append);

//Reads content of a file
std::string readFile(std::string path);

//Gets user input, exports as a string. Allows options for multiline input
std::string getUserInput(bool multiline);

//Basically clears the cin buffer
void ignoreLine();

//Converts a string pulled from a txt or csv file and converts it to a vector
std::vector<std::vector<std::string>> parseCSV(std::string strInputCSV, char cDelimiter, char cBreak, char cEscape);

//Prints a matrix to the console, separated by tab
void printMatrix(std::vector<std::vector<std::string>> vMatrix);

//Performs a find/replace on a string
std::string findReplace(std::string strInput, char cOldChar, std::string strNewStr);

//Exports a matrix to a string value with delimiters and line breaks
std::string matrixToString(std::vector<std::vector<std::string>> vMatrix, char cDelimiter, char cBreak);





#endif // !FILE_FUNCTIONS_H


