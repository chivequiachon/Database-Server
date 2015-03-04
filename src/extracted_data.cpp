#include "extracted_data.h"
#include <string>

void ExtractedData::setData(std::string data)
{
	this->data = data;
}

std::string ExtractedData::getData() const
{
	return this->data;
}

void ExtractedData::setEncryptedID(std::string encryptedID)
{
	this->encryptedID = encryptedID;
}

std::string ExtractedData::getEncryptedID() const
{
	return this->encryptedID;
}

void ExtractedData::setRequestType(int requestType)
{
	//if(requestType > 3 || requestType < 0) requestType = 0;
	this->requestType = requestType;
}

int ExtractedData::getRequestType() const
{
	return this->requestType;
}


ExtractedData& ExtractedData::operator=(const ExtractedData &ed)
{
	this->encryptedID = ed.encryptedID;
	this->data = ed.data;
	this->requestType = ed.requestType;

	return *this;
}