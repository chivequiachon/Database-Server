#ifndef BIEX_H
#define BIEX_H

#include <exception>

class BadValue : public std::exception
{
public:
	inline BadValue() {}
	inline ~BadValue() {}

	virtual const char* what() const throw();
};

class BadSize : public std::exception
{
public:
	inline BadSize() {}
	inline ~BadSize() {}

	virtual const char* what() const throw();
};

#endif