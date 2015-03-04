#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <netinet/in.h>

#include "Server.h"

#include <string>

class TCPServer : public IServer
{
private:
    // sockets
    int listenSocket, clientSocket;
	
	struct sockaddr_in serverAddress;

public:
    inline explicit TCPServer() : listenSocket(-1), clientSocket(-1) {}
    virtual ~TCPServer();

    int getClientSocket() const;
	virtual int& getListenSocket();

    virtual bool setAddressInfo(std::string host, std::string port);

    virtual bool createListeningSocket();
    virtual bool bindSocketToPort();
    virtual bool listenForConnections();
    virtual bool acceptClientSocket();

	virtual int receiveData(const int &clientSocket, unsigned char *recvBuffer, const int buffSize);
	virtual int sendData(const int &clientSocket, const unsigned char *sendBuffer, const int buffSize);

    virtual int receiveData(unsigned char *recvBuffer, const int buffSize);
    virtual int sendData(const unsigned char *sendBuffer, const int buffSize);

    virtual bool shutdownSockets();
    virtual bool closeSockets();
};

#endif