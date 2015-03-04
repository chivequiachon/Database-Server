#include "Server.h"
#include "TCPServer.h"

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <iostream>
#include <cstring>
#include <string>

TCPServer::~TCPServer()
{
    shutdownSockets();
    closeSockets();
}

int& TCPServer::getListenSocket()
{
	return listenSocket;
}

int TCPServer::getClientSocket() const
{
	return clientSocket;
}

bool TCPServer::setAddressInfo(std::string host, std::string port)
{
    /*************************************
                SOCKET(A)
    **************************************/
    /** Set socket address information **/		
	// set the first n bytes to zero
 	bzero((char *) &serverAddress, sizeof(serverAddress));
	
	serverAddress.sin_family = AF_INET;
 	serverAddress.sin_addr.s_addr = INADDR_ANY;
 	serverAddress.sin_port = htons(atoi(port.c_str()));

    return true;
}

bool TCPServer::createListeningSocket()
{
    /*************************************
                SOCKET(B)
    **************************************/
    /** Create a SOCKET for connecting to client **/
    // create socket for this server by using the the address info used for
    // resolving server address and port then use this for listening later
	listenSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
 	
	// check if socket creation is successful
 	if(listenSocket < 0)
		return false;
	
	// set socket option to indicate if local socket address can be reused
	int on = 1;
	if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) < 0)
	{
		std::cout << "reusable local socket address failed." << std::endl;
		return false;
	}


	// set socket option to set the receive timeout value
	//int dwTime = 500; // 0.5 seconds
	struct timeval tv;

	tv.tv_sec = 1;  // 1 second
	tv.tv_usec = 0;  // 500 microseconds

	if(setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval)) < 0)
	{
		std::cout << "socket time set failed." << std::endl;
		return false;
	}

    return true;
}

bool TCPServer::bindSocketToPort()
{
     /*************************************
                    BIND
    **************************************/
    /** Bind listening socket to port **/
    // bind the newly created listening socket using the address
    // info used in resolving the server address and port
	if (bind(listenSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
		return false;

    return true;
}

bool TCPServer::listenForConnections()
{
   /*************************************
                    LISTEN
    **************************************/
    /** Setup the TCP listening socket **/
    // listen for incoming connections
	// with a queue size of 10
	if(listen(this->listenSocket, 3) < 0)
	{
		return false;
	}

	return true;
}

bool TCPServer::acceptClientSocket()
{
   /*************************************
                    ACCEPT
    **************************************/
    /** Accept all incoming connections **/
    // Accept a client socket
	//struct sockaddr_in cli_addr;
	//socklen_t clilen = sizeof(cli_addr);
	
    while(true)
    {
        clientSocket = accept(this->listenSocket, NULL, NULL);
    
        if(clientSocket < 0) continue;

        // check if accept hook has a callable object
        try
        {
            (this->getAcceptHook())(clientSocket);
            return true;
        }
        catch(std::bad_function_call a)
        {
            return false;
        }

	}
    
    /*
    if(clientSocket < 0)
    {
        closeSockets();
        return false;
    }
    */
	

    close(this->listenSocket);
    return true;
}

int TCPServer::receiveData(const int &clientSocket, unsigned char *recvBuffer, const int buffSize)
{
    /*************************************
                    RECEIVE
    **************************************/
    /** Receive all data sent by the client **/
    // Receive the data sent by the client
    // and store it in a buffer
	int flag = recv(clientSocket, recvBuffer, buffSize, MSG_DONTWAIT);

	//return !(flag <= 0);
	return flag;
}

int TCPServer::sendData(const int &clientSocket, const unsigned char *sendBuffer, const int buffSize)
{
    /*************************************
                    SEND
    **************************************/
    /** Send data to the client **/
    // Send to client
	int flag = send(clientSocket, sendBuffer, buffSize, 0);
	return flag;
}

int TCPServer::receiveData(unsigned char *recvBuffer, const int buffSize)
{
    /*************************************
                    RECEIVE
    **************************************/
    /** Receive all data sent by the client **/
    // Receive the data sent by the client
    // and store it in a buffer
    int flag = recv(clientSocket, recvBuffer, buffSize, MSG_DONTWAIT);

    //return !(flag <= 0);
    return flag;
}

int TCPServer::sendData(const unsigned char *sendBuffer, const int buffSize)
{
    /*************************************
                    SEND
    **************************************/
    /** Send data to the client **/
    // Send to client
    int flag = send(clientSocket, sendBuffer, buffSize, 0);
    return flag;
}

bool TCPServer::shutdownSockets()
{
    /*************************************
                    SHUTDOWN
    **************************************/
    /** Shutdown server's listening socket and
        client's connection socket **/
    // shutdown the connection since we're done
    if(shutdown(clientSocket, SHUT_RDWR) < 0)
		return false;

    return true;
}

bool TCPServer::closeSockets()
{
   /*************************************
                    CLOSE
    **************************************/
    // Close all sockets
    close(this->listenSocket);

    return true;
}