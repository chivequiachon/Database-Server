#include "encrypted_packet.h"

#include <iostream>
#include <cstring>

EncryptedPacket::EncryptedPacket(unsigned char *packet) :
	packet(NULL), packetSize(0)
{
	this->packet = packet;
	this->packetSize = strlen(reinterpret_cast<char*>(packet));
}

EncryptedPacket::~EncryptedPacket()
{
	if(packet != NULL)
		delete this->packet;
}

bool EncryptedPacket::isEmpty()
{
	if(this->packet == NULL)
		return true;
	else
	{
		if(this->packetSize <= 0)
			return true;
		return false;
	}
}

unsigned char EncryptedPacket::at(int index)
{
	if(index > packetSize-1) return -1;
	return packet[index];
}

int EncryptedPacket::size() const
{
	return this->packetSize;
}

void EncryptedPacket::setPacket(unsigned char *packet)
{
	if(this->packet != NULL)
		delete packet;

	this->packet = packet;
	this->packetSize = strlen(reinterpret_cast<char*>(packet));
}

std::string EncryptedPacket::getPacket()
{
	return std::string(reinterpret_cast<char*>(this->packet));
}
