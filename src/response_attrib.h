#ifndef RESP_ATTRIB_H
#define RESP_ATTRIB_H

#include <string>

class ResponseAttribute
{
public:
	ResponseAttribute();
	ResponseAttribute(const ResponseAttribute &other);
	explicit ResponseAttribute(unsigned char *encryptedKey,
							   int socket, int requestType,
							   int valBuffer, bool result);
	inline ~ResponseAttribute()
	{
	}

	void setValueBuffer(int valueBuffer);
	void setRequestType(int requestType);
	void setResult(bool result);
	void setEncryptedKey(unsigned char* encryptedKey);
	void setSocket(int socket);

	int getValueBuffer() const;
	int getRequestType() const;
	int getSocket() const;
	bool getResult() const;
	unsigned char* getEncryptedKey() const;

	ResponseAttribute& operator=(const ResponseAttribute &other);

private:
	int valBuffer;
	int requestType;
	int socket;
	bool result;

	unsigned char encryptedKey[5] = {0};
};

#endif