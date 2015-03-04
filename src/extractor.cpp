#include "extractor.h"
#include "big_int_lib.h"

#include <cstring>
#include <iostream>
#include <algorithm>

std::string Extractor::extractRealID(std::string encryptedID)
{
	std::string householdID;

	BigInteger key1 = std::to_string(static_cast<unsigned char>(encryptedID[0]));
	BigInteger key2 = std::to_string(static_cast<unsigned char>(encryptedID[1]));
	BigInteger key3 = std::to_string(static_cast<unsigned char>(encryptedID[2]));
	BigInteger key4 = std::to_string(static_cast<unsigned char>(encryptedID[3]));
	
	BigInteger key_res = (key1 * key2 * key3) + key4;
	
	householdID = key_res.show();
	
	return householdID;
}

std::string Extractor::extractData(unsigned char *packet)
{
	std::string content = std::string(reinterpret_cast<char*>(packet));
	content = content.substr(9, 4);
	
	content.erase(std::remove(content.begin(), content.end(), '_'), content.end());
    
    return content;
}

std::string Extractor::extractID(unsigned char *packet)
{
	std::string content = std::string(reinterpret_cast<char*>(packet));
	return content.substr(4, 4);
}

int Extractor::extractRequestType(unsigned char *packet)
{
	return static_cast<int>(packet[8]);
}

bool Extractor::run()
{
	return false;
}

bool Extractor::run(EncryptedPacket *in, ExtractedData *out)
{
	std::string encryptedID, data;
	
	int requestType = 0;
	if(!in->isEmpty())
	{
		// get the encrypted ID and perform calculations to
		// extract household ID from it
		encryptedID = Extractor::extractID(
			reinterpret_cast<unsigned char*>(
				const_cast<char*>(in->getPacket().c_str())));
		
		requestType = Extractor::extractRequestType(
			reinterpret_cast<unsigned char*>(
				const_cast<char*>(in->getPacket().c_str())));

		data = "";
		if(in->size() == 14)
			data = Extractor::extractData(
				reinterpret_cast<unsigned char*>(
					const_cast<char*>(in->getPacket().c_str())));

		*out = ExtractedData(requestType, encryptedID, data);
		
		return true;
	}

	return false;
}