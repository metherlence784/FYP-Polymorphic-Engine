#ifndef TEMPLATE_FUNCTIONS_H
#define TEMPLATE_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <sstream>

template<typename T>
std::string convert_num_to_hex(T i)
{
        std::stringstream ss;
        ss << "0x"
                << std::setfill('0') << std::setw(sizeof(T) * 2)
                << std::hex << i;
        return ss.str();
}



#endif // TEMPLATE_FUNCTIONS_H
