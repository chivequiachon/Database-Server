#ifndef MULTICLIENT_H
#define MULTICLIENT_H

#include "Server.h"

#include <string>

class MultiClientServer : public IServer
{
private:
	IServer *wrappee;

public:
	inline explicit MultiClientServer(IServer *wrappee) : wrappee(wrappee) {}
	virtual ~MultiClientServer();

	/** SERVER OPERATIONS **/
    virtual bool setAddressInfo(std::string host, std::string port);
    virtual bool createListeningSocket();
    virtual bool bindSocketToPort();
    virtual bool listenForConnections();
    virtual bool acceptClientSocket();
    virtual int receiveData(const int &clientSocket, unsigned char *recvBuffer, const int buffSize);
	virtual int sendData(const int &clientSocket, const unsigned char *sendBuffer, const int buffSize);
    virtual bool shutdownSockets();
    virtual bool closeSockets();
	virtual int& getListenSocket();
};

#endif