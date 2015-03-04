#include "validator.h"
#include "checksum.h"
#include "encrypted_packet.h"
#include <iostream>
#include <cstring>

bool Validator::run() { return false; }
bool Validator::run(EncryptedPacket *ep)
{
	if(ep != NULL)
	{
		if(!(ep->isEmpty()))
		{
			unsigned char *buffer = new unsigned char[ep->size()]();
			strcpy(reinterpret_cast<char*>(buffer), ep->getPacket().c_str());
			
			// validate headers
			if(buffer[0] == '4' && buffer[1] == 'p' && buffer[2] == '?' && buffer[3] == '2')
			{
				if(ep->size() == 14)
				{
					// validate data using fletcher's checksum algorithm
					if(buffer[13] == vm->calculateChecksum(buffer, 13))
					{
						//delete buffer;
						return true;
					}
				}
				else if(ep->size() == 10)
				{
					if(buffer[9] == vm->calculateChecksum(buffer, 9))
					{
						//delete buffer;
						return true;
					}
				}
				else
				{
					//delete buffer;
					return false;
				}
			}
		}
	}

	return false;
}