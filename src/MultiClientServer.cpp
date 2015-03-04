#include "Server.h"
#include "MultiClientServer.h"

#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include <iostream>

MultiClientServer::~MultiClientServer()
{
	if(wrappee) delete wrappee;
}

bool MultiClientServer::setAddressInfo(std::string host, std::string port)
{
	return wrappee->setAddressInfo(host, port);
}

bool MultiClientServer::createListeningSocket()
{
	return wrappee->createListeningSocket();
}

bool MultiClientServer::bindSocketToPort()
{
	return wrappee->bindSocketToPort();
}

bool MultiClientServer::listenForConnections()
{
	return wrappee->listenForConnections();
}

bool MultiClientServer::acceptClientSocket()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	
	//while(1)
    //{
		int clientSocket = accept(wrappee->getListenSocket(), (struct sockaddr*)&cli_addr, &clilen);
        if(clientSocket < 0) return true;
		
		// check if accept hook has a callable object
		try
		{
			(this->getAcceptHook())(clientSocket);
		}
		catch(std::bad_function_call a)
		{
			return false;
		}
    //}

    //if(wrappee)
    	//close(wrappee->getListenSocket());

    return true;
}

int MultiClientServer::receiveData(const int &clientSocket, unsigned char *recvBuffer, const int buffSize)
{
	return wrappee->receiveData(clientSocket, recvBuffer, buffSize);
}

int MultiClientServer::sendData(const int &clientSocket, const unsigned char *sendBuffer, const int buffSize)
{
	return wrappee->sendData(clientSocket, sendBuffer, buffSize);
}

bool MultiClientServer::shutdownSockets()
{
	return false;
}

bool MultiClientServer::closeSockets()
{
	return false;
}

int& MultiClientServer::getListenSocket()
{
	return wrappee->getListenSocket();
}