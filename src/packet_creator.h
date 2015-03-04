#ifndef PCKCREATE_H
#define PCKCREATE_H

#include "checksum.h"

#include <string>

class PacketCreator
{
	PacketCreator() {}

public:
	static std::string formPacket(const std::string header,
								  const std::string message,
								  const int requestType,
								  ChecksumModule *c);

	static std::string formReadPacket(std::string header,
									  unsigned char *id,
								  	  int requestType,
								  	  ChecksumModule *c);

	static std::string formWritePacket(std::string header,
								  	   unsigned char *id,
								  	   unsigned char *message,
								  	   int requestType,
								  	   ChecksumModule *c);
};

#endif