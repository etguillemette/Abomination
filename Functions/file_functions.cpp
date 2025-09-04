#ifndef FILE_FUNCTIONS_CPP
#define FILE_FUNCTIONS_CPP
#pragma once
#include "file_functions.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include <ctime>


std::string timestamp()
{
	auto start = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(start);

	return std::ctime(&end_time);
}


void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void writeFile(std::string content, std::string path, bool append)
{
	//Filestream for the file we would like to write to
	std::fstream outFile;

	//Checks if the append flag is checked
	if (append == true)
	{
		//Sets the file stream to append
		outFile.open(path, std::ios_base::app);

		//Verifies that the file exists
		if (!outFile)
		{
			std::cerr << "Error: File could not be opened.\n";
		}
		else
		{
			//Adds content to file
			outFile << content;
			outFile.close();
		}

	}
	else
	{
		//Sets the file stream to overwrite 
		outFile.open(path, std::ios::out | std::ios::trunc);
		outFile << content;
		outFile.close();
	}


}



std::string readFile(std::string path)
{
	//Filestream of file we would like to read
	std::ifstream inFile;
	inFile.open(path);

	//Checks that the file in question exists
	if (!inFile)
	{
		std::cerr << "Error: File could not be opened.\n";
		return "";
	}
	else
	{

		//Export string
		std::string data;
		while (inFile)
		{
			//Buffer for reading lines of filestream content
			std::string strLine;

			std::getline(inFile, strLine);
			data += strLine + "\n";
		}

		return data;
	}

	return "";
}

std::string getUserInput(bool multiline)
{
	//The string that will be exported by the function
	std::string strExport;

	//Buffer string to hold lines of the user input
	std::string strUserInput;

	if (multiline)
	{
		//Extracts the line, plus whitespace from the cinput stream
		std::getline(std::cin >> std::ws, strUserInput);
		strExport += strUserInput + "\n";

		return strExport;
	}
	else
	{
		std::cin >> strUserInput;
		std::cin.clear();
		ignoreLine();
		return strUserInput;
	}
}

std::vector<std::vector<std::string>> parseCSV(std::string strInputCSV, char cDelimiter, char cBreak, char cEscape)
{
	using namespace std;

	vector<vector<std::string>> vMatrix;

	//A temporary string that holds the characters before a delimiter has been hit
	string strCell = "";

	//A temporary vector that holds all the individual cells as individual elements
	vector<string> vRow;

	//A flag denoting whether or not an escape character is active
	bool ignore = false;

	//std::cout << strInputCSV << "\n";

	for (int i = 0; i < static_cast<int>(strInputCSV.length()); i++)
	{
		//std::cout << "i == " << i << "\n";
		//std::cout << "strInputCSV[i] == " << strInputCSV[i] << "\n";

		//Flips the escape state when an escape character is hit
		if (strInputCSV[i] == cEscape)
		{
			ignore = !ignore;
		}
		//If the break character is hit (typically a return), will add current strCell to row, then add row to the matrix, then clear both
		else if (strInputCSV[i] == cBreak && !ignore)
		{
			if (strCell.size() > 0)
			{
				vRow.push_back(strCell);
				strCell = "";
				vMatrix.push_back(vRow);
				vRow = {};
			}
		}
		//If the delimiter character is hit (typically ','), will add current strCell to row, then clear strCell
		else if (strInputCSV[i] == cDelimiter && !ignore)
		{
			vRow.push_back(strCell);
			strCell = "";
		}
		//If it's not a delimiter, will simply add the character to the strCell
		else
		{
			strCell += strInputCSV[i];
		}

	}


	return vMatrix;
}

void printMatrix(std::vector<std::vector<std::string>> vMatrix)
{
	for (int r = 0; r < static_cast<int>(vMatrix.size()); r++)
	{
		for (int c = 0; c < static_cast<int>(vMatrix[r].size()); c++)
		{
			std::cout << "|" << vMatrix[r][c] << "|\t\t";
		}
		std::cout << "\n";
	}
}

std::string matrixToString(std::vector<std::vector<std::string>> vMatrix, char cDelimiter, char cBreak)
{
	//The string to be ultimately exported by the function
	std::string strExport = "";

	for (int r = 0; r < static_cast<int>(vMatrix.size()); r++)
	{
		for (int c = 0; c < static_cast<int>(vMatrix[r].size()); c++)
		{
			strExport += vMatrix[r][c];
			//If we're not on the last element in the row, adds a delimiter
			if ((c + 1) != static_cast<int>(vMatrix[r].size()))
			{
				strExport.push_back(cDelimiter);
			}
		}
		strExport += cBreak;
	}
	return strExport;
}


std::string findReplace(std::string strInput, char cOldChar, std::string strNewStr)
{
	//The string that the cleanString function will output to
	std::string output = "";
	for (int i = 0; i < static_cast<int>(strInput.size()); i++)
	{
		if (strInput[i] == cOldChar)
		{
			output += strNewStr;
		}
		else
		{
			output += strInput[i];
		}
	}

	return output;
}





#endif // !FILE_FUNCTIONS
