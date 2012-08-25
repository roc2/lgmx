#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <stdexcept> 


template <typename T>
std::string number_to_string(T val)
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}

template <typename T>
T string_to_number(const std::string &str)
{
	std::istringstream ss(str);
    T result;
    
    if (!(ss >> result))
		throw std::runtime_error("Invalid value");
    
    return result;
}


#endif
