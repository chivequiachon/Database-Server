#ifndef RECEIVER_H
#define RECEIVER_H

#include "systemcomponent.h"
#include "Server.h"
#include "encrypted_packet.h"

class Receiver : public SystemComponent
{
	IServer *s;

public:
	inline explicit Receiver(IServer *s) : s(s) {}
	virtual ~Receiver() {}

	virtual bool run();
	virtual bool run(int socket, EncryptedPacket *&out);
};

#endif