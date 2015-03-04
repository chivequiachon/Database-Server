#include "big_int_lib.h"
#include "big_int_exceptions.h"

#include <locale>
#include <exception>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>

BigInteger::BigInteger(const unsigned char *value)
{
	// check size first
	const unsigned int length = strlen((char*)value);
	if(length > static_cast<unsigned int>(SIZE))
		throw BadSize();

	// initialize digit container
	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
		digits[i] = 0;

	// transfer values in reverse form
	for(unsigned int i = 0; i < length; i++)
		digits[(length-1) - i] = value[i] - 48;
}

std::string BigInteger::show()
{
	// extract string equivalent of value
	std::string value;
	
	for(int i = static_cast<int>(DIGIT_SIZE-1); i >= 0; i--)
		value += std::to_string(digits[i]);
	
	// eliminate gap
	char gap = 0;
	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
	{
		if(value[i] != '0') break;
		gap++;
	}
	
	return value.substr(gap);
}

unsigned int BigInteger::getMaxDigit() const
{
	return SIZE;
}

void BigInteger::setValueFromString(const unsigned char *str)
{
	// check size first
	const unsigned int length = strlen((char*)str);
	if(length > SIZE)
		throw BadSize();

	// check if given value are all number digits
	for(unsigned int i = 0; i < length; i++)
		if(!std::isdigit(str[i]))
			throw BadValue();
	
	// reset digit container
	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
		digits[i] = 0;

	// transfer values in reverse form
	for(unsigned int i = 0; i < length; i++)
		digits[(length-1) - i] = str[i] - 48;
}

void BigInteger::setValueFromString(std::string str)
{
	setValueFromString(str.c_str());
}

void BigInteger::setValue(const unsigned char *val)
{
	// check size first
	const unsigned int length = strlen((char*)val);
	if(length > SIZE)
		throw BadSize();
	
	// reset digit container
	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
		digits[i] = 0;

	// transfer values in reverse form
	for(unsigned int i = 0; i < length; i++)
		digits[(length-1) - i] = val[i];
}


BigInteger& BigInteger::operator=(const BigInteger &bi)
{
	bzero(digits, 50);
	memcpy(digits, bi.digits, 51);

	return *this;
}

BigInteger BigInteger::operator+(const BigInteger &bi)
{
	BigInteger sum;
	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
		sum.digits[i] = digits[i] + bi.digits[i];

	for(unsigned int i = 0; i < DIGIT_SIZE; i++)
	{
		char tmp = sum.digits[i];
		if(tmp > 9)
		{
			sum.digits[i] = tmp % 10;
			sum.digits[i+1] += tmp / 10;
		}
	}

	return sum;
}

BigInteger BigInteger::operator*(const BigInteger &bi)
{
	BigInteger product;
	for(unsigned int i = 0; i < SIZE; i++)
	{
		int carry = 0;
		for(unsigned int j = 0; j < SIZE; j++)
		{
			int tmp_prod =
				(digits[j] * bi.digits[i]) + product.digits[i+j] + carry;
			carry = tmp_prod / 10;
			product.digits[i+j] = tmp_prod % 10;
		}
	}

	return product;
}