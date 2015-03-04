#include "receiver.h"
#include "Server.h"
#include "encrypted_packet.h"

#include <iostream>
#include <cstring>

bool Receiver::run() { return false; }
bool Receiver::run(int socket, EncryptedPacket *&out)
{
	// create buffer
	unsigned char *buffer = new unsigned char[15]();

	// receive the packet itself
	if(s->receiveData(socket, buffer, 15) > 0)
	{
		unsigned char *packet =
			new unsigned char[strlen(reinterpret_cast<char*>(buffer)) + 1];

		strcpy( reinterpret_cast<char*>(packet),
				reinterpret_cast<char*>(buffer) );

		out = new EncryptedPacket(packet);
		delete[] buffer;
		return true;
	}

	delete[] buffer;
	return false;		
}