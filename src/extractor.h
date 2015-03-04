#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "systemcomponent.h"
#include "encrypted_packet.h"
#include "extracted_data.h"
#include <string>

class Extractor : public SystemComponent
{
public:
	inline explicit Extractor() {}
	virtual ~Extractor() {}
	
	static std::string extractID(unsigned char *packet);
	static int extractRequestType(unsigned char *packet);
	static std::string extractData(unsigned char *packet);
	static std::string extractRealID(std::string encryptedID);

	virtual bool run();
	virtual bool run(EncryptedPacket *in, ExtractedData *out);
};


#endif