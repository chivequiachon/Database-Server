#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "systemcomponent.h"
#include "checksum.h"
#include "encrypted_packet.h"

class Validator : public SystemComponent
{
	ChecksumModule *vm;

public:
	inline explicit Validator(ChecksumModule *vm) : vm(vm) {}
	virtual ~Validator() {}

	virtual bool run();
	virtual bool run(EncryptedPacket *ep);
};

#endif