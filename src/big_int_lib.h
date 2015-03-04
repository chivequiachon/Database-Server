#ifndef BIL_H
#define BIL_H

#include <locale>
#include <exception>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>

class BigInteger
{
	unsigned char digits[50] = {0};
	const unsigned int DIGIT_SIZE = 50;
	const unsigned int SIZE = 25;

public:
	inline BigInteger() {}
	inline BigInteger(const BigInteger &bi) { memcpy(digits, bi.digits, 51); }
	BigInteger(const unsigned char *value);
	inline BigInteger(std::string str) :
		BigInteger(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())))
	{}

	inline ~BigInteger() {}

	std::string show();
	unsigned int getMaxDigit() const;

	void setValueFromString(const unsigned char *str);
	void setValueFromString(std::string str);
	void setValue(const unsigned char *val);

	BigInteger& operator=(const BigInteger &bi);
	BigInteger operator+(const BigInteger &bi);
	BigInteger operator*(const BigInteger &bi);
};

#endif