#ifndef _SERVER_H
#define _SERVER_H

#include <string>
#include <functional>

class IServer
{
public:
    inline explicit IServer() {}
    virtual ~IServer() {}

    /** SERVER OPERATIONS **/
    //virtual bool initializeWinSock() = 0;
    virtual bool setAddressInfo(std::string host, std::string port) = 0;
    virtual bool createListeningSocket() = 0;
    virtual bool bindSocketToPort() = 0;
    virtual bool listenForConnections() = 0;
    virtual bool acceptClientSocket() = 0;
    virtual int receiveData(const int &clientSocket, unsigned char *recvBuffer, const int buffSize) = 0;
	virtual int sendData(const int &clientSocket, const unsigned char *sendBuffer, const int buffSize) = 0;
    virtual bool shutdownSockets() = 0;
    virtual bool closeSockets() = 0;

    /** TEMPLATE METHODS **/
    virtual void initServer(std::string host, std::string port);
    virtual void startServer();
	virtual void setAcceptHook(std::function<void(int)> acceptHook);
    virtual void killServer();

	virtual int& getListenSocket() = 0;

    virtual std::function<void(int)>& getAcceptHook();

private:
	std::function<void(int)> acceptHook;
};

#endif
