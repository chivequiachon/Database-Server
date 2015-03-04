#ifndef _CHECKSUM
#define _CHECKSUM


//// CHECKSUM MODULE INTERFACE
//// TO BE USED BY THE VALIDATOR AND FILTER
class ChecksumModule
{
private:
	inline explicit ChecksumModule() {}

	friend class Fletcher;

public:
	virtual ~ChecksumModule() {}
	virtual unsigned char calculateChecksum(unsigned char *packet, int len) = 0;
};

class Fletcher : public ChecksumModule
{
public:
	inline explicit Fletcher() {}
	virtual ~Fletcher() {}

	virtual unsigned char calculateChecksum(unsigned char *packet, int len);
};

#endif