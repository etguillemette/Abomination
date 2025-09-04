#ifndef DATA_FUNCTIONS_CPP
#define DATA_FUNCTIONS_CPP
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <OpenXLSX.hpp>


int indexOf(std::string search, std::vector<std::string> const& data)
{
    int index = -1;
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if (data[i] == search)
        {
            index = i;
            break;
        }
    }

    return index;
}

void thisisatestfunc()
{
    std::cout << "This is a test";
}






#endif // ! DATA_FUNCTIONS_CPP
