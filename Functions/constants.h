#ifndef CONSTANTS_H
#define CONSTANTS_H
#pragma once
#include <string>

namespace paths
{
	//Main folders ------------------------
	
	//Path to the master folder
	extern const std::string g_master = "Master/";

	//Path to the output folder
	extern const std::string g_output = "Output/";

	//Path to the input folder
	extern const std::string g_input = "Input/";

	extern const std::string g_documents = "C:\\Users\\eguillemette\\Documents\\01 Important\\Abomination Software\\Abomination Output Folder\\";

	//Reference Documents ------------------------

	//Path to the master settings document
	extern const std::string g_master_xlsx = g_master + "Settings.xlsx";

	//Pricing Documents ------------------------
	extern const std::string g_pricing_doc = g_master + "POSPricing.xlsx";

	//Other ---------------------------

	extern const std::string g_jotform_editurl = "http://www.jotform.com/edit/";

	//Export logger
	extern const std::string g_logger = g_documents + "logger.txt";

	//Html Export
	extern const std::string g_html = g_output + "errorList.html";

	//Html CSS File
	extern const std::string g_css = "table.css";

}

//A series of "magic numbers"
namespace magic
{
	/* Max rows for settings sheets.
	This is important because OpenXLSX::MAX_ROWS and OpenXLSX::MAX_COLS 
	Will return blank cells as valid, and will end up cycling through MILLIONS
	of cells for simple functions. This value represents a hard limit.
	*/
	extern const int g_maxrows = 20;

	/* Max columns for settings sheets.
	This is important because OpenXLSX::MAX_ROWS and OpenXLSX::MAX_COLS
	Will return blank cells as valid, and will end up cycling through MILLIONS
	of cells for simple functions. This value represents a hard limit.
	*/
	extern const int g_maxcols = 20;


}


#endif // !CONSTANTS_H

