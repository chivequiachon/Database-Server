#include "response_attrib.h"

#include <cstring>
#include <iostream>

ResponseAttribute::ResponseAttribute() :
	valBuffer(-1), requestType(-1), socket(-1), result(false)
{
}

ResponseAttribute::ResponseAttribute(unsigned char *encryptedKey,
									 int socket, int requestType,
									 int valBuffer, bool result) :
	valBuffer(valBuffer), requestType(requestType), socket(socket), result(result)
{
	if(encryptedKey != NULL)
	{
		if(strlen(reinterpret_cast<char*>(encryptedKey)) == 4)
		{
			strcpy(reinterpret_cast<char*>(this->encryptedKey),
					reinterpret_cast<char*>(encryptedKey));
		}
	}
}

ResponseAttribute::ResponseAttribute(const ResponseAttribute &other)
{
	this->valBuffer = other.valBuffer;
	this->requestType = other.requestType;
	this->result = other.result;
	this->socket = other.socket;

	strcpy(reinterpret_cast<char*>(this->encryptedKey),
			reinterpret_cast<char*>(
				const_cast<unsigned char*>(other.encryptedKey)));
}

void ResponseAttribute::setValueBuffer(int valueBuffer)
{
	this->valBuffer = valueBuffer;
}

void ResponseAttribute::setRequestType(int requestType)
{
	this->requestType = requestType;
}

void ResponseAttribute::setResult(bool result)
{
	this->result = result;
}

void ResponseAttribute::setEncryptedKey(unsigned char *encryptedKey)
{
	if(encryptedKey != NULL)
	{
		if(strlen(reinterpret_cast<char*>(encryptedKey)) == 4)
		{
			strcpy(reinterpret_cast<char*>(this->encryptedKey),
					reinterpret_cast<char*>(encryptedKey));
		}
	}
}

void ResponseAttribute::setSocket(int socket)
{
	this->socket = socket;
}

int ResponseAttribute::getValueBuffer() const
{
	return this->valBuffer;
}

int ResponseAttribute::getRequestType() const
{
	return this->requestType;
}

int ResponseAttribute::getSocket() const
{
	return this->socket;
}

bool ResponseAttribute::getResult() const
{
	return this->result;
}

unsigned char* ResponseAttribute::getEncryptedKey() const
{
	unsigned char *val = new unsigned char[5]();
	strcpy(reinterpret_cast<char*>(val),
			reinterpret_cast<char*>(
				const_cast<unsigned char*>(this->encryptedKey)));

	return val;
}

ResponseAttribute& ResponseAttribute::operator=(const ResponseAttribute &other)
{
	this->valBuffer = other.valBuffer;
	this->requestType = other.requestType;
	this->result = other.result;
	this->socket = other.socket;

	strcpy(reinterpret_cast<char*>(this->encryptedKey),
			reinterpret_cast<char*>(
				const_cast<unsigned char*>(other.encryptedKey)));

	return *this;
}