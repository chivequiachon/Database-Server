#include "Server.h"

#include <string>
#include <functional>

void IServer::initServer(std::string host, std::string port)
{
    setAddressInfo(host, port);
    createListeningSocket();
    bindSocketToPort();
    listenForConnections();
}

void IServer::startServer()
{
    //initializeWinSock();
    acceptClientSocket();
}

void IServer::setAcceptHook(std::function<void(int)> acceptHook)
{
	this->acceptHook = acceptHook;
}

void IServer::killServer()
{
    shutdownSockets();
    closeSockets();
}

std::function<void(int)>& IServer::getAcceptHook()
{
	return (this->acceptHook);
}