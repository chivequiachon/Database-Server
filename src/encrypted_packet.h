#ifndef ENCRPCKT_H
#define ENCRPCKT_H

#include <string>
#include <cstring>

class EncryptedPacket
{
	unsigned char *packet = NULL;
	int packetSize = 0;

public:
	EncryptedPacket(unsigned char *packet);

	~EncryptedPacket();

	bool isEmpty();
	int size() const;
	void setPacket(unsigned char *packet);
	std::string getPacket();
	unsigned char at(int index);
};

#endif