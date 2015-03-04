#include "big_int_exceptions.h"

#include <exception>
#include <string>

const char* BadValue::what() const throw()
{
	return std::string("Big integer received an invalid value.").c_str();
}

const char* BadSize::what() const throw()
{
	return std::string("Big integer received a value with invalid size.").c_str();
}