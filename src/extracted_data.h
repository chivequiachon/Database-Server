#ifndef EXTRDATA_H
#define EXTRDATA_H

#include <string>

class ExtractedData
{
	std::string encryptedID;
	std::string data;
	int requestType;

public:
	inline explicit ExtractedData() : encryptedID("NULL"), data("NULL"), requestType(0) {}
	inline explicit ExtractedData(int requestType, std::string encryptedID, std::string data) :
		encryptedID(encryptedID), data(data), requestType(requestType)
	{
	}

	inline ~ExtractedData() {}

	void setData(std::string data);
	std::string getData() const;

	void setEncryptedID(std::string encryptedID);
	std::string getEncryptedID() const;

	void setRequestType(int requestType);
	int getRequestType() const;

	ExtractedData& operator=(const ExtractedData &ed);
};


#endif