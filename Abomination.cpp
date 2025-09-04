// Abomination.cpp : Defines the entry point for the application.
//

#include "Abomination.h"
#include "Functions/constants.h"
#include "Functions/file_functions.h"
#include "Functions/math_functions.h"
#include <string>
#include "Functions/text_functions.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include "Functions/data_functions.h"
#include <ctime>
#include <chrono>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <filesystem>

#include "OpenXLSX.hpp"
#include "XLDocument.hpp"
#include "XLCell.hpp"
#include "XLWorksheet.hpp"

using namespace std;
using namespace OpenXLSX;

class Logger {
public:
	//Global path to the logger file
	std::string _path = paths::g_logger;
	std::string currentTime = timestamp();

	//Adds data to the log
	void log(std::string content)
	{
		writeFile(currentTime + ":  " + content + "\n", _path, true);
	}
};

class WorksheetData {
public:
	//Vector list of headers for the worksheet
	std::vector<std::string> headers;

	//Matrix of data pulled from the worksheet
	std::vector<std::vector<XLCellValue>> data;

	//Returns a cell based on an index of the row number and name of the header column
	XLCellValue cellByHeader(std::string headerStr, int nRow)
	{
		//Index of the column
		int headerIndex = indexOf(headerStr, this->headers);

		//Makes sure the column exists, and the row is not out of bounds
		if (headerIndex != -1 && nRow <= this->data.size())
		{
			return this->data[nRow][headerIndex];
		}
		//Cell is invalid
		else
		{
			return XLCellValue(-1);
		}
	}

	//Returns a cell based on an index of row and column
	XLCellValue cellByIndex(int nRow, int nCol)
	{
		return this->data[nRow][nCol];
	}

	//Returns the index of a header by string
	int getHeaderIndex(std::string headerStr)
	{
		return indexOf(headerStr, this->headers);
	}
};


//Function to rip all the data from a worksheet, stopping at empty cells
WorksheetData ripData(OpenXLSX::XLWorksheet worksheetTab, int headerRow, int refColumn)
{
	//Logger for within function
	Logger fconsole;

	//The final data export object
	WorksheetData worksheetExport;

	//The 2-dimensional matrix of data
	std::vector<std::vector<OpenXLSX::XLCellValue>> exportData;

	//The 1-dimensional vector list of header names
	std::vector<std::string> exportHeaders;

	//Final row in the sheet (not index, starts at 1)
	int endRow = 1;

	//Final column in the sheet (not index, starts at 1)
	int endCol = 1;

	//Data type of the current row in the reference column
	std::string rowDataType = worksheetTab.cell(XLCellReference(1, refColumn)).value().typeAsString();

	//Runs through the rows of the first column, stops when it hits an empty cell
	while (rowDataType != "empty")
	{
		//Increments the row
		endRow++;

		//Checks the datatype of the cell
		rowDataType = worksheetTab.cell(XLCellReference(endRow, refColumn)).value().typeAsString();
	}

	//Data type of the current collumn in the header row
	std::string colDataType = worksheetTab.cell(XLCellReference(headerRow, 1)).value().typeAsString();

	//Runs through the columns of the first row, stops when it hits an empty cell
	while (colDataType != "empty")
	{
		//Adds the current cell to the headers vector
		exportHeaders.push_back(worksheetTab.cell(XLCellReference(headerRow, endCol)).value());

		//Increments the row and checks the data type again
		endCol++;
		colDataType = worksheetTab.cell(XLCellReference(headerRow, endCol)).value().typeAsString();
	}


	//Makes sure that the rows are more than 1,
	if (endRow > 2)
	{

		//Loops through the rows of the sheet, stopping at the maxRow
		for (int r = 2; r < endRow; r++)
		{
			//Placeholder vector that will hold the data for the row, and be pushed onto the export vector
			std::vector<XLCellValue> newRow;
			//Loops through the columns of the sheet
			for (int c = 1; c < endCol; c++)
			{
				newRow.push_back(worksheetTab.cell(XLCellReference(r, c)).value());
			}
			//Adds the current row to the export data vector
			exportData.push_back(newRow);
		}
	}
	//Otherwise, there is no data beyond the headers (if even that)
	else
	{
		//Do nothing
	}

	//Adds the headers and data to the export object
	worksheetExport.headers = exportHeaders;
	worksheetExport.data = exportData;

	return worksheetExport;
}

class classObject {
public:
	//Header info for the class object. Public, protected, constructor, etc.
	std::string _strHeader;
	//Functions list for the class object. Setters, getters, etc.
	std::string _strFunctions;
	//The vector of types of constructor values
	std::vector<std::string> _constructorTypes;
	//The vector of constructor values
	std::vector<std::string> _constructorValues;
	//The vector of constructor default values
	std::vector<std::string> _constructorDefaults;

	classObject(std::string strHeader, std::string strFunctions) {
		_strHeader = strHeader;
		_strFunctions = strFunctions;
	}
};


int main()
{
	//1: Declaring sources ----------------------------------------------------------------------

	Logger console;
	console.log("Abomination script start -------------");

	//The user-selected document that we will be pulling code from
	XLDocument input;

	//The destination document
	XLDocument output;

	//The document where various settings are held
	XLDocument settings;

	//Boolean value that will remain true until the user specifies that there are no additional documents to import
	bool additionalDocs = true;

	//Vector of document title strings
	std::vector <std::string> vImportQueue = {};

	while (additionalDocs)
	{
		//Temporary value to hold the user-input filename that will become the "input" document
		std::string filename;

		std::cout << "What is the name of the file you would like to import? (don't include file extension)\n \n";
		std::cin >> filename;

		std::cout << "Filepath is " << paths::g_documents + "input\\" + filename + ".xlsx" << "\n";

		//Attempts to open the file
		input.open(paths::g_documents + "input\\" + filename + ".xlsx");

		//Checks if the file is valid
		if (input)
		{
			//Finds the index of the filename in the import queue. An index of -1 means it has not been found
			int fileIndex = indexOf(filename, vImportQueue);

			//Checks if the file is unique. An index of -1 means it has not been added
			if (fileIndex == -1)
			{
				//Adds the document to the list of import documents if it is a valid document and unique (not yet added to the queue)
				vImportQueue.push_back(filename);
			}
			else
			{
				std::cout << "The document has already been added.\n";
			}

		}
		else
		{
			std::cout << "The document is not valid.\n";
		}

		input.close();

		//Asks the user whether or not there are additional documents to import
		std::cout << "Are there additional documents that will need to be imported? (y/n)\n \n";

		//Temporary variable for user input (are there additional documents to import?)
		char userInput;
		std::cin >> userInput;

		//If no, sets the additionalDocs flag to false to break the loop and clears the cin
		if (userInput == 'n')
		{
			additionalDocs = false;
			std::cin.clear();
			//ignoreLine();
			break;
		}
		else
		{

			std::cin.clear();
			//ignoreLine();
		}
		//If yes, clears the cin (in the event the user typed in more than one character,
		//not clearing it could result in the additional characters being added to the filename string
	}

	//Loops through all the different files in the import queue
	for (int iQueue = 0; iQueue < static_cast<int>(vImportQueue.size()); iQueue++)
	{


		std::string sCurrentFileName = vImportQueue[iQueue];
		XLDocument xImport;

		xImport.open(paths::g_documents + "input\\" + sCurrentFileName + ".xlsx");

		console.log("Opening file " + sCurrentFileName + "\n");

		//Vector list of all the worksheets (tabs) in the current workbook
		std::vector<std::string> worksheetNames = xImport.workbook().worksheetNames();

		//Creates a folder structure for the Excel document we're working with
		std::filesystem::create_directories(paths::g_documents + "output\\" + sCurrentFileName);


		std::vector<std::string> vClassList;

		auto headersTab = xImport.workbook().worksheet("headers");

		WorksheetData headersData = ripData(headersTab, 1, 1);

		int filenameCol = headersData.getHeaderIndex("Filename");
		int includeCol = headersData.getHeaderIndex("Include");

		std::vector<std::string> headerFilenames;
		std::vector<std::string> headerIncludes;

		for (int h = 0; h < headersData.data.size(); h++) {
			headerFilenames.push_back(headersData.data[h][filenameCol]);
			headerIncludes.push_back(headersData.data[h][includeCol]);
		}

		//Loops through each worksheet (tab) in the current workbook
		for (int iTabs = 0; iTabs < worksheetNames.size(); iTabs++) {

			std::string tabName = worksheetNames[iTabs];
			console.log("Opening tab " + tabName + "\n");

			//Final export string for this tab
			std::string exportStr = "";

			for (int headers = 0; headers < headerFilenames.size(); headers++) {
				if (headerFilenames[headers] == tabName) {
					exportStr += "#ifndef ";
					exportStr += allCaps(tabName);
					exportStr += "_H";
					exportStr += "\n";
					exportStr += "#define ";
					exportStr += allCaps(tabName);
					exportStr += "_H";
					exportStr += "\n";
					exportStr += "#pragma once";
					exportStr += "\n";
					exportStr += "\n";

					std::vector<std::string> includeList;

					includeList = stringSplit(headerIncludes[headers], ',');

					for (int inc = 0; inc < includeList.size(); inc++) {
						exportStr += "#include ";
						exportStr += includeList[inc];
						exportStr += "\n";
					}
				}
			}

			//Checks if this is the headers tab. If so, we're going to treat the data here differently
			if (tabName != "headers") {
				//This is a normal tab
				console.log("Tab #" + to_string(iTabs) + " is a normal tab");

				//The tab we're iterating through
				auto currentTab = xImport.workbook().worksheet(tabName);

				//Data ripped from the current iterated tab we're on
				WorksheetData tabData = ripData(currentTab, 1, 1);

				int classNameCol = tabData.getHeaderIndex("Class Name");
				int functionTypeCol = tabData.getHeaderIndex("Function Type");
				int outputTypeCol = tabData.getHeaderIndex("Output Type");
				int pointerCol = tabData.getHeaderIndex("Pointer");
				int valuePrefixCol = tabData.getHeaderIndex("Value Prefix");
				int valueNameCol = tabData.getHeaderIndex("Value Name");
				int subValueCol = tabData.getHeaderIndex("Sub Value");
				int indexValueTypeCol = tabData.getHeaderIndex("Index Value Type");
				int indexValueCol = tabData.getHeaderIndex("Index Value");
				int matrixValueACol = tabData.getHeaderIndex("Matrix Value A");
				int matrixValueBCol = tabData.getHeaderIndex("Matrix Value B");
				int constructorCol = tabData.getHeaderIndex("Constructor");
				int defaultValueCol = tabData.getHeaderIndex("Default Value");
				int emptyReturnCol = tabData.getHeaderIndex("Empty Return");
				int functionTextCol = tabData.getHeaderIndex("Function Text");
				int descriptionCol = tabData.getHeaderIndex("Description");

				console.log("Set up the index columns");
				console.log("Preparing to loop through the rows in the " + tabName + " tab");

				//List of the name of classes
				std::vector<std::string> classNameList;
				//List of the class objects to export
				std::vector<classObject> classObjectList;



				for (int row = 0; row < tabData.data.size(); row++) {
					console.log("Looping through row " + to_string(row) + " of " + to_string(tabData.data.size()) + " of the " + tabName + " tab.");


					//Export string for the functions of this object
					std::string funcStr = "";
					//Export string for the constructor of this object
					std::string headerStr = "";

					//The name of the new class you're creating functions and properties for
					std::string className = tabData.data[row][classNameCol];
					//The type of functions that need to be created to access the value (valueName)
					std::string functionType = tabData.data[row][functionTypeCol];
					//The object type of the the valueName property value
					std::string outputType = tabData.data[row][outputTypeCol];
					//YES or NO, is this valueName property a pointer reference?
					std::string pointer = tabData.data[row][pointerCol];
					//The prefix ('i', 'str', 'vec', etc.) associated with the valueName property
					std::string valuePrefix = tabData.data[row][valuePrefixCol];
					//The name of the object property
					std::string valueName = tabData.data[row][valueNameCol];
					//The type of object that gets exported when you access a map or vector from the valueName property
					std::string subValue = tabData.data[row][subValueCol];
					//The type of index value type for the indexValue used to iterate through the valueName map (if applicable)
					std::string indexValueType = tabData.data[row][indexValueTypeCol];
					//The value used to iterate through the valueName map (if applicable)
					std::string indexValue = tabData.data[row][indexValueCol];
					//The name of the first iterator used to loop through a vector matrix (if applicable). Usually "row"
					std::string matrixValueA = tabData.data[row][matrixValueACol];
					//The name of the second iterator used to loop through a vector matrix (if applicable). Usually "column"
					std::string matrixValueB = tabData.data[row][matrixValueBCol];
					//YES or NO, should this value be in the class constructor?
					std::string constructor = tabData.data[row][constructorCol];
					//The default object constructor for this property (if applicable)
					std::string defaultType = tabData.data[row][defaultValueCol].typeAsString();
					//std::string defaultValue = tabData.data[row][defaultValueCol];
					//std::string defaultValue = tabData.data[row][defaultValueCol];

					std::string defaultValue = "";

					XLCellValue defaultVal = tabData.data[row][defaultValueCol];
					
					if (defaultVal.typeAsString() == "integer") {
						console.log("Default value (" + valueName + ") is an integer");
						int defaultInt = defaultVal;
						defaultValue += to_string(defaultInt);
					}
					if (defaultVal.typeAsString() == "float") {
						console.log("Default value (" + valueName + ") is a float");
						float defaultFloat = defaultVal;
						defaultValue += to_string(defaultFloat);
					}
					if (defaultVal.typeAsString() == "char") {
						console.log("Default value (" + valueName + ") is a character");
						char defaultChar = defaultVal;
						defaultValue += defaultChar;
					}
					if (defaultVal.typeAsString() == "string") {
						console.log("Default value (" + valueName + ") is a string");
						std::string defaultStr = defaultVal;
						defaultValue += defaultStr;
					}
					if (defaultVal.typeAsString() == "boolean") {
						console.log("Default value (" + valueName + ") is a boolean");
						bool defaultBool = defaultVal;
						defaultValue += defaultBool;
					}


					console.log("className = " + className);
					console.log("outputType = " + outputType);
					console.log("default type is: " + defaultType);
					console.log("default value is: " + defaultValue + "\n");


					//The default value for this object when empty
					std::string emptyReturn = tabData.data[row][emptyReturnCol];
					//If this is a function (not a property), this is the code that should be copy-pasted into C++
					std::string functionText = tabData.data[row][functionTextCol];
					//The description of this property
					std::string description = tabData.data[row][descriptionCol];

					if (functionType == "GetSet") {
						funcStr +=
							"\t" + static_cast<std::string>("//Returns the <#PROPERTY> <#OUTPUTTYPE> value") + "\n" +
							"\t" + "<#OUTPUTTYPE> get<#UPVALUENAME>() {" + "\n" +
							"\t" + "\t" + "return <#PROPERTY>;" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Sets the <#PROPERTY> <#OUTPUTTYPE> value" + "\n" +
							"\t" + "void set<#UPVALUENAME>(<#OUTPUTTYPE> <#VALUEPREFIX><#UPVALUENAME>) {" + "\n" +
							"\t" + "\t" + "<#PROPERTY> = <#VALUEPREFIX><#UPVALUENAME>;" + "\n" +
							"\t" + "}" + "\n";
					}
					if (functionType == "Map") {
						funcStr +=
							"\t" + static_cast<std::string>("//Returns the object reference by the <#INDEXVALUE> <#INDEXVALUETYPE> key") + "\n" +
							"\t" + "<#SUBVALUE>* get<#UPSUBVALUE>RefBy<#UPINDEXVALUE>(<#INDEXVALUETYPE> <#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "if(<#PROPERTY>.count(<#INDEXVALUE>)) {" + "\n" +
							"\t" + "\t" + "\t" + "return &<#PROPERTY>[<#INDEXVALUE>];" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "return nullptr;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Adds <#SUBVALUE> to the <#PROPERTY>" + "\n" +
							"\t" + "void add<#UPSUBVALUE>To<#UPCLASSNAME>(<#SUBVALUE> <#UPSUBVALUE>) {" + "\n" +
							"\t" + "\t" + "size_t <#INDEXVALUE> = <#PROPERTY>.size();" + "\n" +
							"\t" + "\t" + "<#UPSUBVALUE>.set<#UPINDEXVALUE>(static_cast<<#INDEXVALUETYPE>>(<#INDEXVALUE>));" + "\n" +
							"\t" + "\t" + "<#PROPERTY>.insert({<#INDEXVALUE>, <#UPSUBVALUE>});\t" + "}" + "\n" +
							"\n" +
							"\t" + "//Returns the <#PROPERTY> object" + "\n" +
							"\t" + "std::map<<#INDEXVALUETYPE>, <#SUBVALUE>>* get<#UPVALUENAME>() {" + "\n" +
							"\t" + "\t" + "return &<#PROPERTY>;" + "\n" +
							"\t" + "}" + "\n";
					}
					if (functionType == "UniqueMap"){
							"\t" + static_cast<std::string>("//Returns the object reference by the <#INDEXVALUE> <#INDEXVALUETYPE> key") + "\n" +
							"\t" + "<#SUBVALUE>* get<#UPSUBVALUE>RefBy<#UPINDEXVALUE>(<#INDEXVALUETYPE> <#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "if(<#PROPERTY>.count(<#INDEXVALUE>)) {" + "\n" +
							"\t" + "\t" + "\t" + "return &<#PROPERTY>[<#INDEXVALUE>];" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "return nullptr;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Returns the <#PROPERTY> object" + "\n" +
							"\t" + "std::map<<#INDEXVALUETYPE>, <#SUBVALUE>>* get<#UPVALUENAME>() {" + "\n" +
							"\t" + "\t" + "return &<#PROPERTY>;" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "<#OUTPUTTYPE> get<#UPVALUENAME>() {" + "\n" +
							"\t" + "\t" + "return <#PROPERTY>;" + "\n" +
							"\t" + "}" +
							"\t" + "void add<#UPSUBVALUE>To<#UPVALUENAME>(loop* <#UPSUBVALUE>) {" + "\n" +
							"\t" + "\t" + "<#INDEXVALUETYPE> <#INDEXVALUE> = <#SUBVALUE>->getId();" + "\n" +
							"\t" + "\t" + "if (<#PROPERTY>.count(<#INDEXVALUE>)) {" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "<#PROPERTY>.insert({ <#INDEXVALUE>, <#SUBVALUE> });" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}";
					}
					if (functionType == "Vector") {
						funcStr +=
							"\t" + static_cast<std::string>("//Returns the whole <#VALUENAME> vector object") + "\n" +
							"\t" + "std::vector<<#SUBVALUE>> get<#UPVALUENAME>() {" + "\n" +
							"\t" + "\t" + "return <#PROPERTY>;" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Returns the nth <#SUBVALUE> by index" + "\n" +
							"\t" + "<#SUBVALUE> get<#UPSUBVALUE>ByIndex(<#INDEXVALUETYPE> <#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "size_t s<#INDEXVALUE> = static_cast<size_t>(<#INDEXVALUE>);" + "\n" +
							"\t" + "\t" + "if (<#PROPERTY>.size() >= s<#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "\t" + "return <#PROPERTY>[s<#INDEXVALUE>];" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "return <#DEFAULTVALUE>;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Sets the nth <#SUBVALUE> by index to <#UPSUBVALUE>" + "\n" +
							"\t" + "void set<#UPSUBVALUE>ByIndex(<#SUBVALUE> <#UPSUBVALUE>, <#INDEXVALUETYPE> <#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "size_t s<#INDEXVALUE> = static_cast<size_t>(<#INDEXVALUE>);" + "\n" +
							"\t" + "\t" + "if (<#PROPERTY>.size() >= s<#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "\t" + "<#PROPERTY>[s<#INDEXVALUE>] = <#UPSUBVALUE>;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Adds a <#SUBVALUE> to the <#VALUENAME> vector object at the end" + "\n" +
							"\t" + "void add<#UPSUBVALUE>ToEnd(<#SUBVALUE> <#UPSUBVALUE>) {" + "\n" +
							"\t" + "\t" + "<#PROPERTY>.push_back(<#UPSUBVALUE>);" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Adds a <#SUBVALUE> to the <#VALUENAME> vector object at the beginning" + "\n" +
							"\t" + "void add<#UPSUBVALUE>ToStart(<#SUBVALUE> <#UPSUBVALUE>) {" + "\n" +
							"\t" + "\t" + "<#PROPERTY>.insert(<#PROPERTY>.begin(), <#UPSUBVALUE>);" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Adds a <#SUBVALUE> to the <#VALUENAME> vector object at the <#INDEXVALUE> index" + "\n" +
							"\t" + "void add<#UPSUBVALUE>ToIndex(<#SUBVALUE> <#UPSUBVALUE>, <#INDEXVALUETYPE> <#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "size_t s<#INDEXVALUE> = static_cast<size_t>(<#INDEXVALUE>);" + "\n" +
							"\t" + "\t" + "if (<#PROPERTY>.size() >= s<#INDEXVALUE>) {" + "\n" +
							"\t" + "\t" + "<#PROPERTY>.insert(<#PROPERTY>.begin() + <#INDEXVALUE>, <#UPSUBVALUE>);" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Clears the <#VALUENAME> vector" + "\n" +
							"\t" + "void clear<#UPVALUENAME>(){" + "\n" +
							"\t" + "\t" + "<#PROPERTY> = {};\t" + "}" + "\n";
					}
					if (functionType == "RefMap") {
						funcStr +=
							"\t" + static_cast<std::string>("//Returns the object reference by the <#INDEXVALUE> <#INDEXVALUETYPE> key") + "\n" +
							"\t" + "<#SUBVALUE>* get<#UPSUBVALUE>RefBy<#UPINDEXVALUE>(<#INDEXVALUETYPE> <#INDEXVALUE>) {\t" + "\t" + "" + "\n" +
							"\t" + "\t" + "if (<#PROPERTY>.count(<#INDEXVALUE>)) {" + "\n" +
							"\t" + "\t" + "\t" + "return <#PROPERTY>[<#INDEXVALUE>];" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "return nullptr;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Adds <#SUBVALUE> to the <#PROPERTY>" + "\n" +
							"\t" + "void add<#UPSUBVALUE>To<#UPCLASSNAME>(<#SUBVALUE>* <#UPSUBVALUE>) {" + "\n" +
							"\t" + "\t" + "int  <#INDEXVALUE> = <#UPSUBVALUE>->get<#UPINDEXVALUE>();" + "\n" +
							"\t" + "\t" + "<#PROPERTY>.insert({ <#INDEXVALUE>, <#UPSUBVALUE> });" + "\n" +
							"\t" + "}" + "\n";
					}
					if (functionType == "Matrix") {
						funcStr +=
							"\t" + static_cast<std::string>("//Returns the <#SUBVALUE> in the <#VALUENAME> object by <#MATRIXVALUEA> and <#MATRIXVALUEB>") + "\n" +
							"\t" + "<#SUBVALUE> get<#UPVALUENAME>By<#UPMATRIXVALUEA>And<#UPMATRIXVALUEB>(<#INDEXVALUETYPE> <#MATRIXVALUEA>, <#INDEXVALUETYPE> <#MATRIXVALUEB>) {" + "\n" +
							"\t" + "\t" + "<#INDEXVALUETYPE> matrixHeight = static_cast<<#INDEXVALUETYPE>>(<#PROPERTY>.size());" + "\n" +
							"\t" + "\t" + "if (matrixHeight >= <#MATRIXVALUEA>) {" + "\n" +
							"\t" + "\t" + "\t" + "<#INDEXVALUETYPE> matrixWidth = static_cast<<#INDEXVALUETYPE>>(<#PROPERTY>[<#MATRIXVALUEA>].size());" + "\n" +
							"\t" + "\t" + "\t" + "if (matrixWidth >= <#MATRIXVALUEA>) {" + "\n" +
							"\t" + "\t" + "\t" + "\t" + "return <#PROPERTY>[<#MATRIXVALUEA>][<#MATRIXVALUEB>];" + "\n" +
							"\t" + "\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "return <#SUBVALUE>(<#EMPTYRETURN>);" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Returns a vector of <#SUBVALUE> objects in the <#PROPERTY> object by <#MATRIXVALUEA>" + "\n" +
							"\t" + "std::vector<<#SUBVALUE>> get<#UPSUBVALUE>By<#UPMATRIXVALUEA>(<#INDEXVALUETYPE> <#MATRIXVALUEA>) {" + "\n" +
							"\t" + "\t" + "int matrixHeight = static_cast<<#INDEXVALUETYPE>>(<#PROPERTY>.size());" + "\n" +
							"\t" + "\t" + "if (matrixHeight >= <#MATRIXVALUEA>) {" + "\n" +
							"\t" + "\t" + "\t" + "return <#PROPERTY>[<#MATRIXVALUEA>];" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "else {" + "\n" +
							"\t" + "\t" + "\t" + "std::vector<<#SUBVALUE>> emptyVec = {};" + "\n" +
							"\t" + "\t" + "\t" + "return emptyVec;" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "}" + "\n" +
							"\t" + "//Returns a matrix of <#SUBVALUE> by <#MATRIXVALUEA>" + "\n" +
							"\t" + "std::vector<<#SUBVALUE>> get<#UPVALUENAME>By<#UPMATRIXVALUEB>(<#INDEXVALUETYPE> <#MATRIXVALUEB>) {" + "\n" +
							"\t" + "\t" + "std::vector<<#SUBVALUE>> exportVec = {};" + "\n" +
							"\t" + "\t" + "<#INDEXVALUETYPE> matrixHeight = static_cast<<#INDEXVALUETYPE>>(<#PROPERTY>.size());" + "\n" +
							"\t" + "\t" + "for (<#INDEXVALUETYPE> r = 0; r < matrixHeight; r++) {" + "\n" +
							"\t" + "\t" + "\t" + "<#INDEXVALUETYPE> rowWidth = static_cast<<#INDEXVALUETYPE>>(<#PROPERTY>[r].size());" + "\n" +
							"\t" + "\t" + "\t" + "if (rowWidth >= <#MATRIXVALUEB>) {" + "\n" +
							"\t" + "\t" + "\t" + "\t" + "exportVec.push_back(<#PROPERTY>[r][<#MATRIXVALUEB>]);" + "\n" +
							"\t" + "\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "}" + "\n" +
							"\t" + "\t" + "exportVec.push_back(<#SUBVALUE>{ {} });" + "\n" +
							"\t" + "}" + "\n";
					}
					if (functionType == "Function") {

						funcStr += "\n";
						funcStr += functionText;
						funcStr += "\n";
					}

					replaceAll(funcStr, "<#CLASSNAME>", className);
					replaceAll(funcStr, "<#UPCLASSNAME>", capStr(className));
					replaceAll(funcStr, "<#OUTPUTTYPE>", outputType);
					replaceAll(funcStr, "<#VALUEPREFIX>", valuePrefix);
					replaceAll(funcStr, "<#UPVALUEPREFIX>", capStr(valuePrefix));
					replaceAll(funcStr, "<#VALUENAME>", valueName);
					replaceAll(funcStr, "<#UPVALUENAME>", capStr(valueName));
					replaceAll(funcStr, "<#PROPERTY>", "_" + valueName);
					replaceAll(funcStr, "<#SUBVALUE>", subValue);
					replaceAll(funcStr, "<#UPSUBVALUE>", capStr(subValue));
					replaceAll(funcStr, "<#INDEXVALUETYPE>", indexValueType);
					replaceAll(funcStr, "<#INDEXVALUE>", indexValue);
					replaceAll(funcStr, "<#UPINDEXVALUE>", capStr(indexValue));
					replaceAll(funcStr, "<#MATRIXVALUEA>", matrixValueA);
					replaceAll(funcStr, "<#UPMATRIXVALUEA>", capStr(matrixValueA));
					replaceAll(funcStr, "<#MATRIXVALUEB>", matrixValueB);
					replaceAll(funcStr, "<#UPMATRIXVALUEB>", capStr(matrixValueB));
					replaceAll(funcStr, "<#DEFAULTVALUE>", defaultValue);
					replaceAll(funcStr, "<#EMPTYRETURN>", emptyReturn);
					replaceAll(funcStr, "<#DESCRIPTION>", description);
					replaceAll(funcStr, "<#>", "\t");


					//Checks to make sure this isn't a function-only row
					if (className != "NULL" && functionType != "Function") {
						headerStr +=
							"\t" + static_cast<std::string>("//") + description + "\n" +
							"\t" + static_cast<std::string>("") + outputType + " _" + valueName + ";" + "\n";
					}

					//Index of this class in the classNameList, if applicable
					int classIndex = indexOf(className, classNameList);


					//Checks if this class has already been established as part of the classObjectList
					if (classIndex == -1) {
						//This is a new unindexed classObject
						classNameList.push_back(className);
						classObject newClassObject(headerStr, funcStr);

						if (constructor == "YES") {
							newClassObject._constructorTypes.push_back(outputType);
							newClassObject._constructorValues.push_back(valueName);
							newClassObject._constructorDefaults.push_back(defaultValue);
						}

						classObjectList.push_back(newClassObject);

					}
					else {
						//This is a classObject that has been already tallied. Adding values to the previous object
						classObjectList[classIndex]._strHeader += headerStr;
						classObjectList[classIndex]._strFunctions += funcStr;

						if (constructor == "YES") {
							classObjectList[classIndex]._constructorTypes.push_back(outputType);
							classObjectList[classIndex]._constructorValues.push_back(valueName);
							classObjectList[classIndex]._constructorDefaults.push_back(defaultValue);
						}

					}
				}


				for (int c = 0; c < classObjectList.size(); c++) {

					if (classNameList[c] != "NULL") {
						exportStr +=
							"class " + classNameList[c] + " {" + "\n" +
							"protected:" + "\n";

						exportStr += "\n";
						exportStr += classObjectList[c]._strHeader;
						exportStr += "\n";
						exportStr += "\n";
						exportStr += "public:" + static_cast<std::string>("\n");
						exportStr += "\n";
						exportStr += "\t";
						exportStr += "//Default constructor";
						exportStr += "\n";
						exportStr += "\t";
						exportStr += classNameList[c];
						exportStr += "(";

						for (int con = 0; con < classObjectList[c]._constructorTypes.size(); con++) {
							exportStr += classObjectList[c]._constructorTypes[con];
							exportStr += " ";
							exportStr += capStr(classObjectList[c]._constructorValues[con]);
							
							if (classObjectList[c]._constructorDefaults[con] != "") {
								exportStr += " = ";
								exportStr += classObjectList[c]._constructorDefaults[con];
							}
							

							if (con != (classObjectList[c]._constructorTypes.size() - 1)) {
								exportStr += ", ";
							}
						}

						exportStr += "){";
						exportStr += "\n";

						for (int con = 0; con < classObjectList[c]._constructorValues.size(); con++) {
							exportStr += "\t";
							exportStr += "\t";
							exportStr += "_";
							exportStr += classObjectList[c]._constructorValues[con];
							exportStr += " = ";
							exportStr += capStr(classObjectList[c]._constructorValues[con]);
							exportStr += ";";
							exportStr += "\n";
						}

						exportStr += "\t";
						exportStr += "}";
						exportStr += "\n";
						exportStr += "\n";
						exportStr += "\t";
						exportStr += "//FUNCTIONS";
						exportStr += "\n";
						exportStr += classObjectList[c]._strFunctions;
						exportStr += "};";
						exportStr += "\n";
						exportStr += "\n";
					}
					else {
						exportStr += "\n";
						exportStr += classObjectList[c]._strFunctions;
						exportStr += "\n";
					}

				}

				exportStr += "#endif";

				//Path to the header export file
				std::string headerPath = paths::g_documents + "output\\" + sCurrentFileName + "/" + tabName + ".h";
				writeFile(exportStr, headerPath, false);

			}
			else {
				//This is a headers tab
				console.log("Tab #" + to_string(iTabs) + " is a header tab");
			}
		}
	}

	return 0;
}


